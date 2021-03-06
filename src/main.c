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
			case 'h':
			case 'v':
			default: usage();
		}
	}

	char filename[256] = "";
	strcat(filename, argv[optind]);

	TermSize ts;
	getTermSize(&ts);

	int img_width, img_height, chann;
	unsigned char* img = stbi_load(filename, &img_width, &img_height, &chann, 0);

	if(img == NULL)
	{
		fprintf(stderr, "\t\033[31;1mError while reading \"%s\"%s\n", filename, RESET);
		exit(1);
	}

	float img_aspect = (float)img_width / (float)img_height;

	size_t image_size = img_width * img_height * chann;

	float h_const = (float)img_width / (float)ts.cols, v_const = (float)img_height / (float)ts.rows;

	// Again just for horizontal
	int r_width, r_height;
	size_t resized_img_size;
	unsigned char* resized_img = NULL;

	// If the image is smaller than the terminal, don't do anything
	if(img_width < ts.cols && img_height < ts.rows)
	{
		r_width = img_width;
		r_height = img_height;
	}
	else
	{
		// Resize
		if(img_aspect >= 1.f)	 // Image is Horizontal
		{
			r_width = ts.cols;
			r_height = img_height / h_const;
		}
		else
		{	 // Image is Vertical
			r_height = ts.rows;
			r_width = img_width / v_const;
		}
	}
	resized_img_size = r_width * r_height * chann;
	resized_img = malloc(resized_img_size);

	stbir_resize_uint8(img, img_width, img_height, 0, resized_img, r_width, r_height, 0, chann);

	uint32_t n = 0U;

	for(const unsigned char* p = resized_img; p < resized_img + resized_img_size - chann; ++n)
	{
		color_t color;
		char str[64];
		setColorP(&color, p);
		const unsigned char* next_row = p + (chann * r_width);
		if(next_row < resized_img + resized_img_size - chann)
		{
			setBg(&color, next_row[0], next_row[1], next_row[2]);
			getStr(&color, str);
			if(chann == 4)
			{
				if(*(p + 3U) != 0U)
					printf("%s\u2580", str);
				else
					printf("%s ", RESET);
			}
			else
			{
				printf("%s\u2580", str);
			}
		}
		else
			goto end;

		p += chann;
		if(n && !((n + 1) % r_width))
		{
			p += chann * r_width;
			printf("\n");
		}
	}
end:
	printf("%s\n", RESET);

	stbi_image_free(img);
	stbi_image_free(resized_img);
	return 0;
}

void usage(void)
{
	fprintf(stderr, "\tUsage: primg \"filename.extension\"\n%s\nVersion: v0.1\n", RESET);
	exit(1);
}

void getTermSize(TermSize* ts)
{
	struct winsize size = {};
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	ts->rows = size.ws_row;
	ts->cols = size.ws_col;
	ts->x_pixels = size.ws_xpixel;
	ts->y_pixels = size.ws_ypixel;
}
