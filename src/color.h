#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include <stdio.h>

typedef struct color_t
{
	uint8_t fr, fg, fb;
	uint8_t br, bg, bb;
} color_t;

void setColor(color_t* color, uint8_t r, uint8_t g, uint8_t b)
{
	color->fr = r;
	color->fg = g;
	color->fb = b;

	color->br = r;
	color->bg = g;
	color->bb = b;
}

void setColorP(color_t* color, const uint8_t* p)
{
	color->fr = p[0];
	color->fg = p[1];
	color->fb = p[2];

	color->br = p[0];
	color->bg = p[1];
	color->bb = p[2];
}

void setRGB(
	color_t* color, uint8_t fr, uint8_t fg, uint8_t fb, uint8_t br, uint8_t bg, uint8_t bb)
{
	color->fr = fr;
	color->fg = fg;
	color->fb = fb;

	color->br = br;
	color->bg = bg;
	color->bb = bb;
}

void getStr(color_t* color, char* str)
{
	sprintf(str,
			"\033[38;2;%hhu;%hhu;%hhum"
			"\033[48;2;%hhu;%hhu;%hhum",
			color->fr,
			color->fg,
			color->fb,

			color->br,
			color->bg,
			color->bb);
}

const char RESET[] = "\033[38m\033[48m\033[m";

#endif

