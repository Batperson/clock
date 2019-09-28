/*
 * graphics.h
 *
 *  Created on: 25/09/2019
 */

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

typedef uint16_t Colour;

static Colour ALWAYS_INLINE RGB(uint8_t r, uint8_t g, uint8_t b)
{
	return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

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


void InitDisplay();
void ClearScreen();
void SetForegroundColour(Colour fg);
void SetBackgroundColour(Colour bg);
void SetFont(PFont pf);
void SetPixel(uint16_t l, uint16_t t);
void DrawRect(uint16_t l, uint16_t t, uint16_t w, uint16_t h);
void DrawText(uint16_t l, uint16_t t, char* psz);
void DrawGradientVertical(uint16_t l, uint16_t t, uint16_t w, uint16_t h, uint16_t ofs);
Colour Gradient(double ratio);

#endif /* GRAPHICS_H_ */
