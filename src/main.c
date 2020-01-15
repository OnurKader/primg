#include "color.h"

#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void usage(void);

typedef struct TermSize
{
	uint16_t rows;
	uint16_t cols;
	uint16_t x_pixels;
	uint16_t y_pixels;
} TermSize;

void getTermSize(TermSize*);

int main(int argc, char** argv)
{
	int opt;
	while((opt = getopt(argc, argv, "")) != -1)
	{
		switch(opt)
		{
			case 'c': break;
			default: usage();
		}
	}

	char filename[256] = "";
	if(argc > 1)
		strcat(filename, argv[optind]);

	TermSize ts;
	getTermSize(&ts);

	int width, height, chann;
	unsigned char* img = stbi_load(filename, &width, &height, &chann, 0);
	if(img == NULL)
	{
		fprintf(stderr, "\t\033[31;1mError while reading %s\033[m\n", filename);
		exit(1);
	}

	float img_aspect = (float)width / (float)height,
		  term_aspect = (float)ts.x_pixels / (float)ts.y_pixels;

	size_t image_size = width * height * chann;

	printf("Image: %s\nWidth: %d\nHeight: %d\nNo. of channels: %d\n",
		   filename,
		   width,
		   height,
		   chann);

	printf("rows: %hu\ncols: %hu\nx_pixels: %hu\ny_pixels: %hu\n",
		   ts.rows,
		   ts.cols,
		   ts.x_pixels,
		   ts.y_pixels);

	const uint8_t greyscale = (chann == 1 || chann == 2);	 // Less than 3 <3 💜

	if(img_aspect >= 1.f && term_aspect >= 1.f)
	{
		uint32_t n = 0U;
		// TODO Handle image being smaller than terminal window
		for(unsigned char* p = img; p < img + image_size; ++n)
		{
			color_t color;
			char str[64];
			setColorP(&color, p);
			getStr(&color, str);
			printf("%s\u2588", str);

			// Skip the ratio of image to term
			for(int i = 0; i < ceilf((float)width / (float)ts.x_pixels); ++i)
				p += chann;
			if(n % ts.cols == 0)
				p += chann * ts.cols;
		}
	}

	/* for(unsigned char* p = img; p != img + image_size; p += chann) */
	/* { */
	/* 	color_t color; */
	/* 	char str[64]; */
	/* 	setColorP(&color, p); */
	/* 	getStr(&color, str); */
	/* 	printf("%s\u2588", str); */
	/* } */

	stbi_image_free(img);
	return 0;
}

void usage(void)
{
	fprintf(stderr, "Usage: primg \"filename.extension\"\n");
	exit(1);
}

void getTermSize(TermSize* ts)
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	ts->rows = size.ws_row;
	ts->cols = size.ws_col;
	ts->x_pixels = size.ws_xpixel;
	ts->y_pixels = size.ws_ypixel;
}
