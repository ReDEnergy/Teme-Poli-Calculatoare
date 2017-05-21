/**
 * Tema 3 ASC - TV Channel Preview Rendering
 * Gabriel Ivanica 333CA
 */

#include <spu_intrinsics.h>
#include <spu_mfcio.h>
#include <libmisc.h>

#include "tema3_spu.h"

#define waitag(t) mfc_write_tag_mask(1<<t); mfc_read_tag_status_all();

unsigned char *imgdata;
unsigned char *imgscale;
info_t pf __attribute__ ((aligned(16)));
uint32_t tag_id;
render_info RI;

/**
 * Return a new reserved tag
 */
uint32_t reserveTag() {

	uint32_t tag_id = mfc_tag_reserve();

	if (tag_id == MFC_TAG_INVALID) {
		printf("SPU: ERROR can't allocate tag ID\n");
		return -1;
	}

	return tag_id;
}


/**
 * Get Initial Info
 */
void getInitialParameters(unsigned long long argp, unsigned long long envp) {

	/* Initial transfer for getting the info_t struct */
	mfc_get((void*)&pf, (uint64_t) argp, (uint32_t)envp, tag_id, 0, 0);
	waitag(tag_id);

	// Compute some variables that will be needed in the future
	RI.slices = pf.height / NUM_STREAMS;
	RI.line_size = pf.width * NUM_CHANNELS;
	RI.get_size = RI.line_size * SCALE_FACTOR;
	RI.stream_width = pf.width / SCALE_FACTOR;

	// Alloc aligned memory for DMA transfer
	imgdata = malloc_align(RI.get_size, 4);
	imgscale = malloc_align(RI.get_size / 4, 4);
}

/**
 * Render an image using vectors for fast
 */
void  renderImageSlice(int stream) {

	unsigned int i;
	int j, k, val;
	int vectors_in_line = RI.line_size / 16;
	int offset = stream * RI.stream_width * NUM_CHANNELS;

	// Vector pointers
	vector unsigned char *pvimg = (vector unsigned char *)imgdata;
	vector unsigned char *pv1;
	vector unsigned char *pv2;

	// Add the columns using vectors
	for (i = 0; i < 2 ; i++) {
		pv1 = pvimg + vectors_in_line * 2 * i;
		pv2 = pvimg + vectors_in_line * (2 * i + 1);
		for (j = 0; j < vectors_in_line; j++)
			pv1[j] = spu_avg(pv1[j], pv2[j]);
	}

	// Get the final avg line
	for (j = 0; j < vectors_in_line; j++)
		pvimg[j] = spu_avg(pvimg[j], pv1[j]);

	// Avg each 4 pixels from the line
	for(i = 0; i < RI.stream_width; i++) {
		for (k = 0; k < NUM_CHANNELS; k++) {
			val = 0;
			for (j = 0; j < SCALE_FACTOR; j++)
				val	+= imgdata[i * 12 + j * 3 + k];
			imgscale[3 * i + k + offset] = val / 4;
		}
	}
}

/**
 * Render the output for this SPU
 * Each SPU will render 1/16 from the final preview image
 * The SPU has access to 4 pointers to images
 * The SPU will render 1/4 from each image based on its SPU_ID_NUMBER
 */
void renderImage() {

	unsigned int i;
	// Stream number
	int st = 0;

	// Offset for sending data to SPU
	int put_offset = pf.spuid * RI.slices * RI.line_size;

	// Offset for accesing 4 lines on each DMA
	int get_offset = (pf.spuid % 4) * RI.slices * RI.get_size;

	for (i = 0; i < RI.slices; i++) {
		for (st = 0; st < NUM_IMAGES_WIDTH; st++) {
			// Get next 4 lines from the image from stream st
			mfc_get((void*)imgdata, (uint32_t)(pf.img[st] + get_offset), RI.get_size, tag_id, 0, 0);
			waitag(tag_id);

			// Render the 4 lines of data into 1 line (1/4 width)
			renderImageSlice(st);
		}

		// Send rendered image ( 1/16 from the final render )
		mfc_put((void*)imgscale, (uint32_t)(pf.render + put_offset), RI.line_size, tag_id, 0, 0);
		waitag(tag_id);

		// Increment
		put_offset += RI.line_size;
		get_offset += RI.get_size;
	}
}



int main(unsigned long long speid, unsigned long long argp, unsigned long long envp)
{
	unsigned int i;
	unsigned int data;

	tag_id = reserveTag();

	// Get initial parameters
	getInitialParameters(argp, envp);

	for (i = 0; i < pf.frames; i++) {

		// Wait for event from PPU to beging rendering
		while (spu_stat_in_mbox() <= 0);
		data = spu_read_in_mbox();

		// Render the output for this SPU
		renderImage();

		// Notify PPU that image render is done
		while (spu_stat_out_intr_mbox() <= 0);
		spu_write_out_intr_mbox(data);
	}

	// Free memory
	free_align(imgdata);
	free_align(imgscale);

	return 0;
}

/**
 * Log Info Structure
 * 		initial structures with access information
 */
void logInfoStructure() {

	fprintf(stderr, "SPU[%u] :: w: %u h: %u frames: %d render:%p\n",
			pf.spuid, pf.width, pf.height, pf.frames, pf.render);

	fprintf(stderr, "SPU[%u] :: %d %d %d %d\n",
			pf.spuid, pf.stream[0], pf.stream[1], pf.stream[2], pf.stream[3]);

	fprintf(stderr, "SPU[%u] :: %p %p %p %p\n",
			pf.spuid, pf.img[0], pf.img[1], pf.img[2], pf.img[3]);

	fprintf(stderr, "SPU[%u] :: imgdata: %p imgscale: %p\n",
			pf.spuid, imgdata, imgscale);
}
