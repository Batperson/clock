/*
 * graphics_impl.h
 *
 *  Created on: 29/11/2019
 */

#ifndef GRAPHICS_IMPL_H_
#define GRAPHICS_IMPL_H_

typedef struct
{
	uint16_t	l;
	uint16_t	t;
	uint16_t	w;
	uint16_t	h;
	PFont		ft;
	Colour		fg;
	Colour		bg;
	uint16_t	loff;
	uint16_t	toff;
} DrawOp, *PDrawOp;

void SetPixelImpl(PDrawOp pd);
void DrawRectImpl(PDrawOp pd);
void DrawTextImpl(PDrawOp pd, const char* psz);
void DrawBitmapImpl(PDrawOp pd, uint16_t l, uint16_t t, PBitmap bm);

#endif /* GRAPHICS_IMPL_H_ */
