/*
 * graphics.c
 *
 *  Created on: 28/09/2019
 */
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "macros.h"
#include "system.h"
#include "graphics.h"
#include "graphics_impl.h"
#include "fonts.h"

static DrawOp op = {
	0, 0, 0, 0,
	NULL,
	RGB(0, 255, 0),
	RGB(0, 0, 0),
	0, 0
};

static Colour fg	= RGB(0, 255, 0);
static Colour bg	= RGB(0, 0, 0);

void SetForegroundColour(Colour f) { fg = f; }
void SetBackgroundColour(Colour b) { bg = b; }
void SetFont(PFont f) {  op.ft = f; }

Colour Gradient(double ratio)
{
	// Credit: Barmak Shemirani
	// https://stackoverflow.com/questions/40629345/fill-array-dynamicly-with-gradient-color-c

    //we want to normalize ratio so that it fits in to 6 regions
    //where each region is 256 units long
	uint32_t normalized = (uint32_t)(ratio * 256 * 6);

    //find the distance to the start of the closest region
	uint32_t x = normalized % 256;

	uint8_t red = 0, grn = 0, blu = 0;
    switch(normalized / 256)
    {
		case 0: red = 255;      grn = x;        blu = 0;       break;	// red
		case 1: red = 255 - x;  grn = 255;      blu = 0;       break;	// yellow
		case 2: red = 0;        grn = 255;      blu = x;       break;	// green
		case 3: red = 0;        grn = 255 - x;  blu = 255;     break;	// cyan
		case 4: red = x;        grn = 0;        blu = 255;     break;	// blue
		case 5: red = 255;      grn = 0;        blu = 255 - x; break;	// magenta
    }

    return RGB(red, grn, blu);
}

Colour FadeColour(Colour fg, uint8_t intensity)
{
	// intensity = 0 - 16
	uint32_t 		r	= (((fg >> 11) 	& 0x1F) * intensity) / 16;
	uint32_t		g	= (((fg >> 5)  	& 0x3F) * intensity) / 16;
	uint32_t		b	= ((fg 			& 0x1F) * intensity) / 16;

	return (r << 11) | (g << 5) | b;
}

void MeasureChar(char c, uint16_t* w, uint16_t* h)
{
	*w	= op.ft->width;
	*h	= op.ft->height;
}

void MeasureText(const char* psz, uint16_t* w, uint16_t* h)
{
	*w	= strlen(psz) * op.ft->width;
	*h	= op.ft->height;
}

void SetPixel(uint16_t l, uint16_t t, DrawFlags flags)
{
	op.l	= l;
	op.t	= t;
	op.fg	= (flags & DrawInverse) ? bg : fg;
	op.bg	= (flags & DrawInverse) ? fg : bg;

	SetPixelImpl(&op);
}

void DrawRect(uint16_t l, uint16_t t, uint16_t w, uint16_t h, DrawFlags flags)
{
	op.l	= l;
	op.t	= t;
	op.w	= w;
	op.h	= h;
	op.fg	= (flags & DrawInverse) ? bg : fg;
	op.bg	= (flags & DrawInverse) ? fg : bg;

	DrawRectImpl(&op);
}

void DrawText(uint16_t l, uint16_t t, uint16_t w, uint16_t h, DrawFlags flags, const char* psz)
{
	uint16_t actw, acth;	// actual width & height

	MeasureText(psz, &actw, &acth);

	op.fg			= (flags & DrawInverse) ? bg : fg;
	op.bg			= (flags & DrawInverse) ? fg : bg;

	if(flags & FillMargin)
	{
		if(acth < h)
		{
			op.l 	= l;
			op.t	= t;
			op.w	= w;
			op.h	= h - acth;

			switch(flags & (AlignBottom | AlignVCentre))
			{
			case AlignBottom:
				DrawRectImpl(&op);
				break;

			case AlignVCentre:
				op.h	= (h - acth) / 2;
				DrawRectImpl(&op);

				op.t	= t + op.h + acth;
				if((h - acth) & 0x01)
					op.h += 1;

				DrawRectImpl(&op);
				break;

			default:
				op.t	= t + acth;
				DrawRectImpl(&op);
				break;
			}
		}

		if(actw < w)
		{
			op.l	= l;
			op.t	= t;
			op.h	= h;
			op.w	= w - actw;

			switch(flags & (AlignRight | AlignCentre))
			{
			case AlignRight:
				DrawRectImpl(&op);
				break;

			case AlignCentre:
				op.w	= (w - actw) / 2;
				DrawRectImpl(&op);

				op.l	= l + op.w + actw;
				if((w - actw) & 0x01)
					op.w += 1;

				DrawRectImpl(&op);
				break;

			default:
				op.l	= l + actw;
				DrawRectImpl(&op);
				break;
			}
		}
	}

	switch(flags & (AlignRight | AlignCentre))
	{
	case AlignRight:
		op.loff			= (actw > w) ? actw - w : 0;
		op.l			= (actw > w) ? l : l + (w - actw);
		op.w			= (actw > w) ? w : actw;
		break;
	case AlignCentre:
		op.loff			= (actw > w) ? (actw - w) / 2 : 0;
		op.l			= (actw > w) ? l : l + (w - actw) / 2;
		op.w			= (actw > w) ? w : actw;
		break;
	default:
		op.loff			= 0;
		op.l			= l;
		op.w		 	= (actw > w) ? w : actw;
		break;
	}

	switch(flags & (AlignBottom | AlignVCentre))
	{
	case AlignBottom:
		op.toff			= (acth > h) ? acth - h : 0;
		op.t			= (acth > h) ? h : h + (h - acth);
		op.h			= (acth > h) ? h :acth;
		break;
	case AlignVCentre:
		op.toff			= (acth > h) ? (acth - h) / 2 : 0;
		op.t			= (acth > h) ? t : t + (h - acth) / 2;
		op.h			= (acth > h) ? h : acth;
		break;
	default:
		op.toff			= 0;
		op.t			= t;
		op.h			= (acth > h) ? h :acth;
		break;
	}

	while(op.loff && *psz)
	{
		uint16_t cw, ch;
		MeasureChar(*psz, &cw, &ch);

		if(op.loff < cw)
			break;

		op.loff -= cw;
		psz++;
	}

	DrawTextImpl(&op, psz);
}
