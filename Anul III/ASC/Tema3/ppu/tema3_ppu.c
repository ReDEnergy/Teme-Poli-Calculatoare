/**
 * Tema 3 ASC - TV Channel Preview Rendering
 * Gabriel Ivanica 333CA
 */

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <libspe2.h>
#include <sys/time.h>
#include <pthread.h>
#include <malloc.h>

#include "tema3_ppu.h"

#define MAX_SPU_THREADS   16

extern spe_program_handle_t tema3_spu;

// Variables for using MailBoxes with Events
spe_event_unit_t pevents[MAX_SPU_THREADS];
spe_event_unit_t events_received;
spe_event_handler_ptr_t event_handler;
spe_context_ptr_t ctx[MAX_SPU_THREADS];

// SPE Threads
int spu_threads;
pthread_t threads[MAX_SPU_THREADS];

// Info Structure to transmit to SPE
info_t thread_arg[MAX_SPU_THREADS];

// Frames for storing image data
image frame[NUM_STREAMS];
image frame_render;
int num_frames;

// Buffers for files path and name
char buf[MAX_PATH_LEN];
char input_path[MAX_PATH_LEN];


/**
 * Start SPE threads
 */
void *ppu_pthread_function(void *thread_arg) {

	info_t *args = (info_t*) thread_arg;
	int ctxID = args->spuid;

	/* Run SPE context */
	unsigned int entry = SPE_DEFAULT_ENTRY;
	if (spe_context_run (ctx[ctxID], &entry, 0, thread_arg, (void*)sizeof(info_t), NULL) < 0) {
		perror ("Failed running context");
		exit (1);
	}

	pthread_exit(NULL);
}


/**
 * Init SPE threads
 * 		thread arguments
 * 		initializing events
 */
void init_spe_threads() {
	int i, j;

	// Create several SPE-threads to execute.
	for(i = 0; i < spu_threads; i++) {

		// Init arguments for thread creation and init information for SPE
		thread_arg[i].width  = frame[0].width;
		thread_arg[i].height = frame[0].height;
		thread_arg[i].frames = num_frames;
		thread_arg[i].render = frame_render.data;
		thread_arg[i].spuid  = i;

		// Each SPE will get access to 4 STREAMS
		for (j = 0; j < SCALE_FACTOR; j++) {
			thread_arg[i].img[j] = frame[i / 4 * 4 + j].data;
			thread_arg[i].stream[j] = i / 4 * 4 + j;
		}

		// Create SPE context
		if ((ctx[i] = spe_context_create (SPE_EVENTS_ENABLE, NULL)) == NULL) {
			perror ("Failed creating context");
			exit (1);
		}

		// Init Mailbox - Event based
		pevents[i].events = SPE_EVENT_OUT_INTR_MBOX;
		pevents[i].data.u32 = i;
		pevents[i].spe = ctx[i];
		spe_event_handler_register(event_handler, &pevents[i]);

		// Load SPE program into context
		if (spe_program_load (ctx[i], &tema3_spu)) {
			perror ("Failed loading program");
			exit (1);
		}

		// Create thread for each SPE context
		if (pthread_create (&threads[i], NULL, &ppu_pthread_function, &thread_arg[i]))  {
			perror ("Failed creating thread");
			exit (1);
		}
	}
}


/**
 * Load frame in memory for accesing from SPU
 */
void load_frames_in_memory(int frame_nr) {
	int i;
	for (i = 0; i < NUM_STREAMS; i++) {
		sprintf(buf, "%s/stream%02d/image%d.pnm", input_path, i + 1, frame_nr);
		read_pnm(buf, &frame[i]);
	}
}

/**
 * Render a frame on SPU
 */
void render_frame(int frame) {
	int i;
	int response;
	unsigned int data = 1;

	for (i = 0; i < MAX_SPU_THREADS; i++)
		spe_in_mbox_write(ctx[i], &data, 1, SPE_MBOX_ANY_BLOCKING);

	// Wait for SPU-thread to complete render the frame
	for(i = 0; i < MAX_SPU_THREADS; i++) {
		// fprintf(stderr, "Waiting for spe[%d]\n", i);
		response = spe_event_wait(event_handler, &events_received, 1, -1);

		if (response) {
			// fprintf(stderr, "SPU[%d] :: completed rendering\n", events_received.data.u32);
			spe_out_intr_mbox_read (events_received.spe, &data, 1, SPE_MBOX_ANY_BLOCKING);
		}
		else
			fprintf(stderr, "Wait for events failed\n");
	}

	fprintf(stderr, "Completed Frame %d\n", frame);
}

/*******************************************************************************
 ****************************** MAIN FUNCTION **********************************
 ******************************************************************************/

int main(int argc, char **argv) {

	int i;
	struct timeval t1, t2, t3, t4;
	double total_time = 0;
	double scale_time = 0;
	char output_path[MAX_PATH_LEN];

	if (argc != 4) {
		printf("Usage: ./tema3_ppu input_path output_path num_frames\n");
		exit(1);
	}

	strncpy(input_path, argv[1], MAX_PATH_LEN - 1);
	strncpy(output_path, argv[2], MAX_PATH_LEN - 1);
	num_frames = atoi(argv[3]);

	if (num_frames > MAX_FRAMES)
		num_frames = MAX_FRAMES;

	gettimeofday(&t1, NULL);

	/**
	 * Determine the number of SPE threads to create.
	 */
	spu_threads = spe_cpu_info_get(SPE_COUNT_USABLE_SPES, -1);
	if (spu_threads > MAX_SPU_THREADS) spu_threads = MAX_SPU_THREADS;

	/**
	 * Init Mailbox - Event based
	 */
	event_handler = spe_event_handler_create();

	/**
	 * Memory allocation for frames
	 */
	for(i = 0; i < NUM_STREAMS; i++)
		alloc_image(&frame[i], 624, 352);

	load_frames_in_memory(1);

	frame_render.width = frame[0].width;
	frame_render.height = frame[0].height;
	frame_render.data = memalign(16, NUM_CHANNELS * frame[0].width * frame[0].height);

	init_spe_threads();

	// Render the first Frame
	gettimeofday(&t3, NULL);
		render_frame(1);
	gettimeofday(&t4, NULL);
	scale_time += GET_TIME_DELTA(t3, t4);

	//write the rendered frame
	sprintf(buf, "%s/result%d.pnm", output_path, 1);
	write_pnm(buf, &frame_render);

	for(i = 1; i < num_frames; i++) {
		// The next Frame is loaded into memory
		load_frames_in_memory(i + 1);

		// Render the Frame
		gettimeofday(&t3, NULL);
		render_frame(i + 1);
		gettimeofday(&t4, NULL);
		scale_time += GET_TIME_DELTA(t3, t4);

		// Write the Frame
		sprintf(buf, "%s/result%d.pnm", output_path, i + 1);
		write_pnm(buf, &frame_render);
	}

	 // Wait for SPU-threads to complete execution
	for (i = 0; i < spu_threads; i++) {

		// Join threads
		if (pthread_join (threads[i], NULL)) {
			perror("Failed pthread_join");
			exit (1);
		}
	}

	 // Destroy the SPU-threads
	for (i = 0; i < spu_threads; i++) {

		// Destroy context
		if (spe_context_destroy (ctx[i]) != 0) {
			perror("Failed destroying context");
			exit (1);
		}

		// Free image memory
		free_image(&frame[i]);
	}

	// Get the total execution time
	gettimeofday(&t2, NULL);
	total_time += GET_TIME_DELTA(t1, t2);

	printf("Render ended\n");
	printf("Scale time: %lf\n", scale_time);
	printf("Total time: %lf\n", total_time);

	return 0;
}

/*******************************************************************************
 ************************ I/O FUNCTION FOR STREMS ******************************
 ************************ Functions from serial.c ******************************
 ******************************************************************************/

//read a character from a file specified by a descriptor
char read_char(int fd, char* path) {
	char c;
	int bytes_read;

	bytes_read = read(fd, &c, 1);
	if (bytes_read != 1){
		PRINT_ERR_MSG_AND_EXIT("Error reading from %s\n", path);
	}

	return c;
}

//allocate image data
void alloc_image(image* img, int width, int height) {
	img->data = memalign(16, NUM_CHANNELS * width * height);

	if (!img->data){
		PRINT_ERR_MSG_AND_EXIT("memalign failed\n");
	}
}

//free image data
void free_image(image* img){
	free(img->data);
}

/* read from fd until character c is found
 * result will be atoi(str) where str is what was read before c was
 * found
 */
unsigned int read_until(int fd, char c, char* path) {

	char buf[SMALL_BUF_SIZE];
	int i;
	unsigned int res;

	i = 0;
	memset(buf, 0, SMALL_BUF_SIZE);
	buf[i] = read_char(fd, path);
	while (buf[i] != c){
		i++;
		if (i >= SMALL_BUF_SIZE){
			PRINT_ERR_MSG_AND_EXIT("Unexpected file format for %s\n", path);
		}
		buf[i] = read_char(fd, path);
	}
	res = atoi(buf);
	if (res <= 0) {
		PRINT_ERR_MSG_AND_EXIT("Result is %d when reading from %s\n",
			res, path);
	}

	return res;
}

//read a pnm image
void read_pnm(char* path, image* img) {
	int fd, bytes_read, bytes_left;
	char image_type[IMAGE_TYPE_LEN];
	unsigned char *ptr;
	unsigned int max_color;

	fd = open(path, O_RDONLY);

	if (fd < 0){
		PRINT_ERR_MSG_AND_EXIT("Error opening %s\n", path);
		exit(1);
	}

	//read image type; should be P6
	bytes_read = read(fd, image_type, IMAGE_TYPE_LEN);
	if (bytes_read != IMAGE_TYPE_LEN){
		PRINT_ERR_MSG_AND_EXIT("Couldn't read image type for %s\n", path);
	}
	if (strncmp(image_type, "P6", IMAGE_TYPE_LEN)){
		PRINT_ERR_MSG_AND_EXIT("Expecting P6 image type for %s. Got %s\n",
			path, image_type);
	}

	//read \n
	read_char(fd, path);

	//read width, height and max color value
	img->width = read_until(fd, ' ', path);
	img->height = read_until(fd, '\n', path);
	max_color = read_until(fd, '\n', path);
	if (max_color != MAX_COLOR){
		PRINT_ERR_MSG_AND_EXIT("Unsupported max color value %d for %s\n",
			max_color, path);
	}

	//read the actual data
	bytes_left = img->width * img->height * NUM_CHANNELS;
	ptr = img->data;
	while (bytes_left > 0){
		bytes_read = read(fd, ptr, bytes_left);
		if (bytes_read <= 0){
			PRINT_ERR_MSG_AND_EXIT("Error reading from %s\n", path);
		}
		ptr += bytes_read;
		bytes_left -= bytes_read;
	}

	close(fd);
}

//write a pnm image
void write_pnm(char* path, image* img) {
	int fd, bytes_written, bytes_left;
	char buf[32];
	unsigned char* ptr;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0){
		PRINT_ERR_MSG_AND_EXIT("Error opening %s\n", path);
	}

	//write image type, image width, height and max color
	sprintf(buf, "P6\n%d %d\n%d\n", img->width, img->height, MAX_COLOR);
	ptr = (unsigned char*)buf;
	bytes_left = strlen(buf);
	while (bytes_left > 0) {
		bytes_written = write(fd, ptr, bytes_left);
		if (bytes_written <= 0){
			PRINT_ERR_MSG_AND_EXIT("Error writing to %s\n", path);
		}
		bytes_left -= bytes_written;
		ptr += bytes_written;
	}

	//write the actual data
	ptr = img->data;
	bytes_left = img->width * img->height * NUM_CHANNELS;
	while (bytes_left > 0){
		bytes_written = write(fd, ptr, bytes_left);
		if (bytes_written <= 0){
			PRINT_ERR_MSG_AND_EXIT("Error writing to %s\n", path);
		}
		bytes_left -= bytes_written;
		ptr += bytes_written;
	}

	close(fd);
}

