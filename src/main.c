#include "color.h"

#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_resize.h"

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
	if(argc < 2)
	{
		fprintf(stderr, "\t\033[31;1mWrong number of arguments\n");
		usage();
	}

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
	strcat(filename, argv[optind]);

	TermSize ts;
	getTermSize(&ts);

	int width, height, chann;
	unsigned char* img = stbi_load(filename, &width, &height, &chann, 0);

	if(img == NULL)
	{
		fprintf(stderr, "\t\033[31;1mError while reading \"%s\"%s\n", filename, RESET);
		exit(1);
	}

	float img_aspect = (float)width / (float)height,
		  term_aspect = (float)ts.x_pixels / (float)ts.y_pixels;

	size_t image_size = width * height * chann;

	float h_const = (float)width / (float)ts.cols, v_const = (float)height / (float)ts.rows;

	// Again just for horizontal
	int r_width = ts.cols, r_height = height / h_const;
	size_t resized_img_size = r_width * r_height * chann;
	unsigned char* resized_img = malloc(resized_img_size);

	// Resize
	stbir_resize_uint8(img, width, height, 0, resized_img, r_width, r_height, 0, chann);

	if(img_aspect >= 1.f && term_aspect >= 1.f)
	{
		uint32_t n = 0U;
		// TODO Handle image being smaller than terminal window

		for(const unsigned char* p = resized_img;
			p < resized_img + resized_img_size - chann;
			++n)
		{
			color_t color;
			char str[64];
			setColorP(&color, p);
			const unsigned char* next_row = p + (chann * r_width);
			if(next_row < resized_img + resized_img_size)
			{
				setBg(&color, next_row[0], next_row[1], next_row[2]);
				getStr(&color, str);
			}
			else
			{
				/* setBg(&color, 100, 225, 255); */
				getStr(&color, str);
				strcat(str, "\033[48;2m");
			}
			printf("%s\u2580", str);

			if(n % r_width == 0)
				p += chann * r_width;
			p += chann;
		}
	}
	printf("%s\n", RESET);

	stbi_image_free(img);
	stbi_image_free(resized_img);
	return 0;
}

void usage(void)
{
	fprintf(stderr, "\tUsage: primg \"filename.extension\"\n%s", RESET);
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
