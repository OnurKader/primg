#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void usage(void);

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
	strcat(filename, argv[optind]);

	int width, height, chann;
	unsigned char* img = stbi_load(filename, &width, &height, &chann, 0);
	if(img == NULL)
	{
		fprintf(stderr, "\t\033[31;1mError while reading %s\033[m\n", filename);
		exit(1);
	}
	size_t image_size = width * height * chann;

	int greyscale = (chann == 1 || chann == 2);	   // Less than 3 <3 💜
	for(const char* p = filename; *p < image_size; p += chann) {}

	printf("Image: %s\nWidth: %d\nHeight: %d\nNo. of channels: %d\n",
		   filename,
		   width,
		   height,
		   chann);

	stbi_image_free(img);
	return 0;
}

void usage(void)
{
	fprintf(stderr, "Usage: primg \"filename.extension\"\n");
	exit(1);
}

