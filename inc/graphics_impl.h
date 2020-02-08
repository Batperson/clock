/*
 * graphics_impl.h
 *
 *  Created on: 29/11/2019
 */

#ifndef GRAPHICS_IMPL_H_
#define GRAPHICS_IMPL_H_

typedef struct
{
	Colour fore;
	Colour back;
} Colours;

typedef Colours (*brushcallback_ptr)(uint16_t l, uint16_t t);

typedef struct
{
	uint16_t			l;
	uint16_t			t;
	uint16_t			w;
	uint16_t			h;
	PFont				ft;
	Colour				fg;
	Colour				bg;
	uint16_t			loff;
	uint16_t			toff;
	brushcallback_ptr	pbr;
} DrawOp, *PDrawOp;

void SetPixelImpl(PDrawOp pd);
void DrawRectImpl(PDrawOp pd);
void DrawTextImpl(PDrawOp pd, const char* psz);
void DrawTextExImpl(PDrawOp pd, const char* psz);
void DrawBitmapImpl(PDrawOp pd, uint16_t l, uint16_t t, PBitmap bm);
void DrawBitmapExImpl(PDrawOp pd, uint16_t l, uint16_t t, PBitmap bm);

#endif /* GRAPHICS_IMPL_H_ */
