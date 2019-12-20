/*
 * graphics.h
 *
 *  Created on: 25/09/2019
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef uint16_t Colour;

#define RGB(r, g, b) (((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3))

#define BLACK           0x0000
#define BLUE            0x001F
#define GREEN           0x07E0
#define CYAN            0x07FF
#define RED             0xF800
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

typedef struct
{
	uint8_t width;
	uint8_t height;
	uint8_t cellwidth;
	uint8_t	offset;
	uint8_t data[];
} Font, *PFont;

typedef enum {
	AlignLeft		= 0x00,
	AlignRight		= 0x01,
	AlignCentre		= 0x02,
	AlignTop		= 0x00,
	AlignBottom		= 0x04,
	AlignVCentre	= 0x08,

	DrawNormal		= 0x000,
	DrawInverse		= 0x800,

	FillMargin		= 0x1000
} DrawFlags;

void InitDisplay();
void ClearScreen();
void SetForegroundColour(Colour fg);
void SetBackgroundColour(Colour bg);
void SetFont(PFont pf);
void SetPixel(uint16_t l, uint16_t t, DrawFlags flags);
void SetTextRangeColour(uint16_t st, uint16_t len, Colour fg);
void MeasureChar(char c, uint16_t* w, uint16_t* h);
void MeasureText(const char* psz, uint16_t* w, uint16_t* h);
void DrawRect(uint16_t l, uint16_t t, uint16_t w, uint16_t h, DrawFlags flags);
void DrawText(uint16_t l, uint16_t t, uint16_t w, uint16_t h, DrawFlags flags, const char* psz);
void DrawGradientVertical(uint16_t l, uint16_t t, uint16_t w, uint16_t h, uint16_t ofs);
void DrawGradientHorizontal(uint16_t l, uint16_t t, uint16_t w, uint16_t h, uint16_t ofs);

Colour Gradient(double ratio);

#endif /* GRAPHICS_H_ */
