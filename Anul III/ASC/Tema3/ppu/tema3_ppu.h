/**
 * Tema3 ASC - TV Channel Preview Rendering
 * Ivanica Gabriel 333CA
 */

#include <stdio.h>
#include <stdlib.h>

#define PRINT_ERR_MSG_AND_EXIT(format, ...) \
	{ \
	fprintf(stderr, "%s:%d: " format, __func__, __LINE__, ##__VA_ARGS__); \
	fflush(stderr); \
	exit(1); \
	}

#define NUM_STREAMS 		16
#define MAX_FRAMES			100		// there are at most 100 frames available
#define MAX_PATH_LEN		256
#define IMAGE_TYPE_LEN		2
#define SMALL_BUF_SIZE		16
#define SCALE_FACTOR		4
#define NUM_CHANNELS		3 		// red, green and blue
#define MAX_COLOR			255
#define NUM_IMAGES_WIDTH	4
#define NUM_IMAGES_HEIGHT	4
#define IMG_WIDTH			624
#define IMG_HEIGHT			352

#define GET_COLOR_VALUE(img, i, j, k) \
	((img)->data[((i) * (img->width) + (j)) * NUM_CHANNELS + (k)])

#define RED(img, i, j)		GET_COLOR_VALUE(img, i, j, 0)
#define GREEN(img, i, j)	GET_COLOR_VALUE(img, i, j, 1)
#define BLUE(img, i, j)		GET_COLOR_VALUE(img, i, j, 2)

//macro for easily getting how much time has passed between two events
#define GET_TIME_DELTA(t1, t2) ((t2).tv_sec - (t1).tv_sec + \
				((t2).tv_usec - (t1).tv_usec) / 1000000.0)

/**
 * Data layout for an image:
 * if RED_i, GREEN_i, BLUE_i are the red, green and blue values for
 * the i-th pixel in the image than the data array inside struct image
 * looks like this:
 * RED_0 GREEN_0 BLUE_0 RED_1 GREEN_1 BLUE_1 RED_2 ...
*/
typedef struct _image {
	unsigned int width;
	unsigned int height;
	unsigned char *data;
} __attribute__ ((aligned(16))) image;

/**
 * Struct for transfering information to SPU
 * Pointers for DMA access
 * 		4 pointers to images from where to read
 * 		1 pointer to write the rendered image
 */
typedef struct _info_t {
	unsigned int width;
	unsigned int height;
	unsigned int spuid;
	unsigned int frames;
	unsigned int stream[4];
	unsigned char *img[4];
	unsigned char *render;
} __attribute__ ((aligned(16))) info_t;

/**
 * Function for I/O
 */
char read_char(int fd, char* path);

void alloc_image(image* img, int width, int height);

void free_image(image* img);

void write_pnm(char* path, image* img);

unsigned int read_until(int fd, char c, char* path);

void read_pnm(char* path, image* img);
