/**
 * Tema3 ASC - TV Channel Preview Rendering
 * Ivanica Gabriel 333CA
 */

#include <stdio.h>
#include <stdlib.h>

#define NUM_STREAMS 		16
#define MAX_FRAMES			100		//there are at most 100 frames available
#define MAX_PATH_LEN		256
#define IMAGE_TYPE_LEN		2
#define SMALL_BUF_SIZE		16
#define SCALE_FACTOR		4
#define NUM_CHANNELS		3 		//red, green and blue
#define MAX_COLOR			255
#define NUM_IMAGES_WIDTH	4		// the final big image has 4 small images
#define NUM_IMAGES_HEIGHT	4		// on the width and 4 on the height

#define GET_COLOR_VALUE(img, i, j, k) \
	((img)->data[((i) * (img->width) + (j)) * NUM_CHANNELS + (k)])

#define RED(img, i, j)		GET_COLOR_VALUE(img, i, j, 0)
#define GREEN(img, i, j)	GET_COLOR_VALUE(img, i, j, 1)
#define BLUE(img, i, j)		GET_COLOR_VALUE(img, i, j, 2)

/**
 * Struct need for different variables for the output render
 */
typedef struct _render_info {
	unsigned int stream_width;
	unsigned int slices;
	unsigned int line_size;
	uint32_t get_size;
} render_info;

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
