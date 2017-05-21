// Cod serial Tema 3 ASC 2013
 
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define PRINT_ERR_MSG_AND_EXIT(format, ...) \
	{ \
	fprintf(stderr, "%s:%d: " format, __func__, __LINE__, ##__VA_ARGS__); \
	fflush(stderr); \
	exit(1); \
	}

#define NUM_STREAMS 		16	
#define MAX_FRAMES			100	//there are at most 100 frames available
#define MAX_PATH_LEN		256
#define IMAGE_TYPE_LEN 		2
#define SMALL_BUF_SIZE 		16
#define SCALE_FACTOR		4
#define NUM_CHANNELS		3 //red, green and blue
#define MAX_COLOR			255
#define NUM_IMAGES_WIDTH	4 // the final big image has 4 small images
#define NUM_IMAGES_HEIGHT	4 // on the width and 4 on the height

//macros for easily accessing data
#define GET_COLOR_VALUE(img, i, j, k) \
	((img)->data[((i) * (img->width) + (j)) * NUM_CHANNELS + (k)])
#define RED(img, i, j)		GET_COLOR_VALUE(img, i, j, 0)
#define GREEN(img, i, j)	GET_COLOR_VALUE(img, i, j, 1)
#define BLUE(img, i, j)		GET_COLOR_VALUE(img, i, j, 2)

//macro for easily getting how much time has passed between two events
#define GET_TIME_DELTA(t1, t2) ((t2).tv_sec - (t1).tv_sec + \
				((t2).tv_usec - (t1).tv_usec) / 1000000.0)
				
//structure that is used to store an image into memory
struct image{
	unsigned int width, height;
	unsigned char *data;
};
/* data layout for an image:
 * if RED_i, GREEN_i, BLUE_i are the red, green and blue values for 
 * the i-th pixel in the image than the data array inside struct image
 * looks like this:
 * RED_0 GREEN_0 BLUE_0 RED_1 GREEN_1 BLUE_1 RED_2 ...
*/

//read a character from a file specified by a descriptor
char read_char(int fd, char* path){
	char c;
	int bytes_read;
	
	bytes_read = read(fd, &c, 1);
	if (bytes_read != 1){
		PRINT_ERR_MSG_AND_EXIT("Error reading from %s\n", path);
	}
	
	return c;
}

//allocate image data
void alloc_image(struct image* img){
	img->data = calloc(NUM_CHANNELS * img->width * img->height, sizeof(char));
	
	if (!img->data){
		PRINT_ERR_MSG_AND_EXIT("Calloc failed\n");
	}
}

//free image data
void free_image(struct image* img){
	free(img->data);
}

/* read from fd until character c is found
 * result will be atoi(str) where str is what was read before c was
 * found 
 */
unsigned int read_until(int fd, char c, char* path){

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
void read_pnm(char* path, struct image* img){
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
		
	//allocate image data
	alloc_image(img);
	
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
void write_pnm(char* path, struct image* img){
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
	while (bytes_left > 0){
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

//scale using area averaging
void scale_area_avg(struct image* src, struct image* dest){
	int m, n, i, j;
	
	for (i = 0; i < dest->height; i++){
		for (j = 0; j < dest->width; j++){
			int rval = 0, gval = 0, bval = 0;
			for (m = i * SCALE_FACTOR; m < (i + 1) * SCALE_FACTOR; m++){
				for (n = j * SCALE_FACTOR; n < (j + 1) * SCALE_FACTOR; n++){
					rval += RED(src, m, n);
					gval += GREEN(src, m, n);
					bval += BLUE(src, m, n);
				}
			}
			RED(dest, i, j) = rval / SCALE_FACTOR / SCALE_FACTOR;
			GREEN(dest, i, j) = gval / SCALE_FACTOR / SCALE_FACTOR;
			BLUE(dest, i, j) = bval / SCALE_FACTOR / SCALE_FACTOR;
		}
	}
}

//create final result image from downscaled images
void create_big_image(struct image* scaled, struct image* big_image){
	int i, j, k;
	unsigned char* ptr = big_image->data;
	struct image* img_ptr;
	unsigned int height = scaled[0].height;
	unsigned int width = scaled[0].width;
	
	for (i = 0; i < NUM_IMAGES_HEIGHT; i++){
		for (k = 0; k < height; k++) {
			//line by line copy
			for (j = 0; j < NUM_IMAGES_WIDTH; j++){
				img_ptr = &scaled[i * NUM_IMAGES_WIDTH + j];
				memcpy(ptr, &img_ptr->data[k * width * NUM_CHANNELS], width * NUM_CHANNELS);
				ptr += width * NUM_CHANNELS;
			}
		}
		
	}
}

int main(int argc, char** argv){
	int i, j, num_frames;
	char buf[MAX_PATH_LEN];
	char input_path[MAX_PATH_LEN];
	char output_path[MAX_PATH_LEN];
	struct image input[NUM_STREAMS];
	struct image scaled[NUM_STREAMS];
	struct image big_image;
	
	struct timeval t1, t2, t3, t4;
	double scale_time = 0, total_time = 0;
	
	if (argc != 4){
		printf("Usage: ./serial input_path output_path num_frames\n");
		exit(1);
	}
	
	gettimeofday(&t3, NULL);
	strncpy(input_path, argv[1], MAX_PATH_LEN - 1);
	strncpy(output_path, argv[2], MAX_PATH_LEN - 1);
	num_frames = atoi(argv[3]);

	if (num_frames > MAX_FRAMES)
		num_frames = MAX_FRAMES;
		
	for (i = 0; i < num_frames; i++){
		printf("Processing Frame %d\n", i + 1);
		
		//read the input images
		for (j = 0; j < NUM_STREAMS; j++){
			sprintf(buf, "%s/stream%02d/image%d.pnm", input_path, 
				j + 1, i + 1);
			read_pnm(buf, &input[j]);
		}

		gettimeofday(&t1, NULL);
		//scale the input images
		for (j = 0; j < NUM_STREAMS; j++){
			scaled[j].height = input[j].height / SCALE_FACTOR;
			scaled[j].width = input[j].width / SCALE_FACTOR;
			alloc_image(&scaled[j]);
			scale_area_avg(&input[j], &scaled[j]);
		}
		//create the big image out of the scaled images
		big_image.height = scaled[0].height * NUM_IMAGES_HEIGHT;
		big_image.width = scaled[0].width * NUM_IMAGES_WIDTH;
		alloc_image(&big_image);
		create_big_image(scaled, &big_image);
		gettimeofday(&t2, NULL);
		scale_time += GET_TIME_DELTA(t1, t2);
		
		//write the big image
		sprintf(buf, "%s/result%d.pnm", output_path, i + 1);
		write_pnm(buf, &big_image);
		
		//free the image data
		for (j = 0; j < NUM_STREAMS; j++){
			free_image(&input[j]);
			free_image(&scaled[j]);
		}
		free_image(&big_image);
	}
	gettimeofday(&t4, NULL);
	total_time += GET_TIME_DELTA(t3, t4);

	printf("Scale time: %lf\n", scale_time);
	printf("Total time: %lf\n", total_time);
	
	return 0;
}
