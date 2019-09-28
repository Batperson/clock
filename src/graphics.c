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
#include "fonts.h"

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
