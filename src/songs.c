/*
 * songs.c
 *
 *  Created on: 14/10/2019
 */

#include "stm32f10x.h"
#include "sound.h"
#include "song.h"

extern Voice sine;

Track scale_c_track_1 = {
	255,
	6,
	&sine,
	{
		{ C4, 4 },
		{ D4, 4 },
		{ E4, 4 },
		{ F4, 4 },
		{ G4, 4 },
		{ A4, 4 },
		{ B4, 4 },
		{ C5, 4 },
		{ 0, 0 }
	}
};


Song scale_c = {
	80,
	{ &scale_c_track_1, 0 }
};
