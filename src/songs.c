/*
 * songs.c
 *
 *  Created on: 14/10/2019
 */

#include <stddef.h>
#include "stm32f10x.h"
#include "sound.h"
#include "song.h"

extern Voice sine;
extern Voice saw;

const Track arpeg_track_1 = {
	255,
	6,
	&saw,
	{
		{ A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT },
		{ A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT },
		{ A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT }, { A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT }, { A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT }, { A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT },
		{ A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT }, { A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT }, { A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT }, { A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT },
		{ A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT },
		{ A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT },
		{ A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT }, { A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT }, { A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT }, { A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT },
		{ A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT }, { A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT }, { A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT }, { A3,  EGHT }, { D4,  EGHT }, { F4s, EGHT },
		{ A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT },
		{ A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT },
		{ B3,  EGHT }, { D4,  EGHT }, { G4,  EGHT }, { B3,  EGHT }, { D4,  EGHT }, { G4,  EGHT }, { B3,  EGHT }, { D4,  EGHT }, { G4,  EGHT }, { B3,  EGHT }, { D4,  EGHT }, { G4,  EGHT },
		{ B3,  EGHT }, { D4,  EGHT }, { G4,  EGHT }, { B3,  EGHT }, { D4,  EGHT }, { G4,  EGHT }, { B3,  EGHT }, { D4,  EGHT }, { G4,  EGHT }, { B3,  EGHT }, { D4,  EGHT }, { G4,  EGHT },
		{ A4,  EGHT }, { C6s, EGHT }, { E6,  EGHT }, { A5,  EGHT }, { C6s, EGHT }, { E6,  EGHT }, { A5,  EGHT }, { C6s, EGHT }, { E6,  EGHT }, { A5,  EGHT }, { C6s, EGHT }, { E6,  EGHT },
		{ A4,  EGHT }, { C5s, EGHT }, { E5,  EGHT }, { A4,  EGHT }, { C5s, EGHT }, { E5,  EGHT }, { A4,  EGHT }, { C5s, EGHT }, { E5,  EGHT }, { A4,  EGHT }, { C5s, EGHT }, { E5,  EGHT },
		{ A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT }, { A3,  EGHT }, { C4s, EGHT }, { E4,  EGHT },
		{ A3,  HLF  },
		{ 0, 0 }
	}
};

const Track arpeg_track_2 = {
	255,
	0,
	&sine,
	{
		{ C5s, WHL  }, { C5s, WHL  },
		{ D5,  WHL  }, { D5,  WHL  },
		{ C5s, WHL  }, { C5s, WHL  },
		{ D5,  WHL  }, { D5,  WHL  },
		{ C5s, WHL  }, { C5s, WHL  },
		{ B4,  WHL  }, { B4,  WHL  },
		{ A4,  WHL  }, { A4,  WHL  },
		{ E4,  WHL  },
		{ A3,  HLF  },
		{ 0, 0 }
	}
};

const Track arpeg_track_3 = {
	255,
	0,
	&sine,
	{
		{ A4,  WHL  }, { A4,  WHL  },
		{ A4,  WHL  }, { A4,  WHL  },
		{ A4,  WHL  }, { A4,  WHL  },
		{ A4,  WHL  }, { A4,  WHL  },
		{ A4,  WHL  }, { A4,  WHL  },
		{ B4,  WHL  }, { B4,  WHL  },
		{ A4,  WHL  }, { A4,  WHL  },
		{ A4,  WHL  },
		{ A4,  HLF  },
		{ 0, 0 }
	}
};

const Track reveille_track_1 = {
	255,
	6,
	&saw,
	{
		{ G3, QRT  },
		{ C4, QRT  }, { E4, EGH  }, { C4, EGH  }, { G3, QRT  }, { E4, QRT  },
		{ C4, QRT  }, { E4, EGH  }, { C4, EGH  }, { G3, QRT  }, { E4, QRT  },
		{ C4, QRT  }, { E4, EGH  }, { C4, EGH  }, { G3, QRT  }, { C4, QRT  },
		{ E4, HLF  }, { C4, QRT  }, { G3, QRT  },
		{ C4, QRT  }, { E4, EGH  }, { C4, EGH  }, { G3, QRT  }, { E4, QRT  },
		{ C4, QRT  }, { E4, EGH  }, { C4, EGH  }, { G3, QRT  }, { E4, QRT  },
		{ C4, QRT  }, { E4, EGH  }, { C4, EGH  }, { G3, QRT  }, { G3, QRT  },
		{ C4, HLFD }, { E4, QRT  },

		{ E4, QRT  }, { E4, QRT  }, { E4, QRT  }, { E4, QRT  },
		{ G4, HLF  }, { E4, QRT  }, { C4, QRT  },
		{ E4, QRT  }, { C4, QRT  }, { E4, QRT  }, { C4, QRT  },
		{ E4, HLF  }, { C4, QRT  }, { E4, QRT  },
		{ E4, QRT  }, { E4, QRT  }, { E4, QRT  }, { E4, QRT  },
		{ G4, HLF  }, { E4, QRT  }, { C4, QRT  },
		{ E4, QRT  }, { C4, QRT  }, { G3, QRT  }, { G3, QRT  },

		{ C4, HLFD }, { G3, QRT  },
		{ C4, QRT  }, { E4, EGH  }, { C4, EGH  }, { G3, QRT  }, { E4, QRT  },
		{ C4, QRT  }, { E4, EGH  }, { C4, EGH  }, { G3, QRT  }, { E4, QRT  },
		{ C4, QRT  }, { E4, EGH  }, { C4, EGH  }, { G3, QRT  }, { C4, QRT  },
		{ E4, HLF  }, { C4, QRT  }, { G3, QRT  },
		{ C4, QRT  }, { E4, EGH  }, { C4, EGH  }, { G3, QRT  }, { E4, QRT  },
		{ C4, QRT  }, { E4, EGH  }, { C4, EGH  }, { G3, QRT  }, { E4, QRT  },
		{ C4, QRT  }, { E4, EGH  }, { C4, EGH  }, { G3, QRT  }, { G3, QRT  },
		{ C4, HLFD },

		{ 0,  0 }
	}
};

const Track bbshark_track_1 = {
	255,
	6,
	&saw,
	{
		{ D4, QRT }, { E4, QRT },
		{ G4, EGH }, { G4, EGH }, { G4, EGH }, { G4, SXT }, { G4, SXT},
		{ G4, SXT }, { G4, SXT }, { G4, EGH }, { D4, EGH }, { E4, EGH },
		{ G4, EGH }, { G4, EGH }, { G4, EGH }, { G4, SXT }, { G4, SXT},
		{ G4, SXT }, { G4, SXT }, { G4, EGH }, { D4, EGH }, { E4, EGH },
		{ G4, EGH }, { G4, EGH }, { G4, EGH }, { G4, SXT }, { G4, SXT},
		{ G4, SXT }, { G4, SXT }, { G4, EGH }, { G4, EGH }, { G4, EGH },
		{ F4s, HLF },

		{ D4, QRT }, { E4, QRT },
		{ G4, EGH }, { G4, EGH }, { G4, EGH }, { G4, SXT }, { G4, SXT},
		{ G4, SXT }, { G4, SXT }, { G4, EGH }, { D4, EGH }, { E4, EGH },
		{ G4, EGH }, { G4, EGH }, { G4, EGH }, { G4, SXT }, { G4, SXT},
		{ G4, SXT }, { G4, SXT }, { G4, EGH }, { D4, EGH }, { E4, EGH },
		{ G4, EGH }, { G4, EGH }, { G4, EGH }, { G4, SXT }, { G4, SXT},
		{ G4, SXT }, { G4, SXT }, { G4, EGH }, { G4, EGH }, { G4, EGH },
		{ F4s, HLF },

		{ D4, QRT }, { E4, QRT },
		{ G4, EGH }, { G4, EGH }, { G4, EGH }, { G4, SXT }, { G4, SXT},
		{ G4, SXT }, { G4, SXT }, { G4, EGH }, { D4, EGH }, { E4, EGH },
		{ G4, EGH }, { G4, EGH }, { G4, EGH }, { G4, SXT }, { G4, SXT},
		{ G4, SXT }, { G4, SXT }, { G4, EGH }, { D4, EGH }, { E4, EGH },
		{ G4, EGH }, { G4, EGH }, { G4, EGH }, { G4, SXT }, { G4, SXT},
		{ G4, SXT }, { G4, SXT }, { G4, EGH }, { G4, EGH }, { G4, EGH },
		{ F4s, HLF },

		{ D4, QRT }, { E4, QRT },
		{ G4, EGH }, { G4, EGH }, { G4, EGH }, { G4, SXT }, { G4, SXT},
		{ G4, SXT }, { G4, SXT }, { G4, EGH }, { D4, EGH }, { E4, EGH },
		{ G4, EGH }, { G4, EGH }, { G4, EGH }, { G4, SXT }, { G4, SXT},
		{ G4, SXT }, { G4, SXT }, { G4, EGH }, { D4, EGH }, { E4, EGH },
		{ G4, EGH }, { G4, EGH }, { G4, EGH }, { G4, SXT }, { G4, SXT},
		{ G4, SXT }, { G4, SXT }, { G4, EGH }, { G4, EGH }, { G4, EGH },
		{ F4s, HLF },

		{ 0,  0 }
	}
};

const Track bbshark_track_2 = {
	255,
	6,
	&sine,
	{
		{  0, HLF },
		{ D4, WHL },
		{ E4, WHL },
		{ G4, WHL },
		{ F4s, HLF },

		{  0, HLF },
		{ G3, EGH }, { G4, EGH }, { G3, EGH }, { G4, EGH },
		{ G3, EGH }, { G4, EGH }, { G3, EGH }, { G4, EGH },
		{ C4, EGH }, { C5, EGH }, { C4, EGH }, { C5, EGH },
		{ C4, EGH }, { C5, EGH }, { C4, EGH }, { C5, EGH },
		{ E4, EGH }, { E5, EGH }, { E4, EGH }, { E5, EGH },
		{ E4, EGH }, { E5, EGH }, { E4, EGH }, { E5, EGH },
		{ D4, EGH }, { C4, EGH }, { B3, EGH }, { A3, EGH },

		{ G3, HLF },
		{ G3, EGH }, { G4, EGH }, { G3, EGH }, { G4, EGH },
		{ G3, EGH }, { G4, EGH }, { G3, EGH }, { G4, EGH },
		{ C4, EGH }, { C5, EGH }, { C4, EGH }, { C5, EGH },
		{ C4, EGH }, { C5, EGH }, { C4, EGH }, { C5, EGH },
		{ E4, EGH }, { E5, EGH }, { E4, EGH }, { E5, EGH },
		{ E4, EGH }, { E5, EGH }, { E4, EGH }, { E5, EGH },
		{ D4, HLF },

		{ G3, HLF },
		{ G3, HLF },
		{ G3, EGH }, { B3, EGH }, { D3, EGH }, { G4, EGH },
		{ C4, HLF },
		{ C4, EGH }, { E4, EGH }, { G4, EGH }, { C5, EGH },
		{ E4, HLF },
		{ E4, EGH }, { G4, EGH }, { C5, EGH }, { E5, EGH },
		{ D4, HLF },

		{ 0,  0 }
	}
};

const Track bbshark_track_3 = {
	255,
	6,
	&sine,
	{
		{  0, HLF },
		{ G4, WHL },
		{ G4, WHL },
		{ C5 /*B4*/, WHL },
		{ A4, HLF },

		{  0, HLF },
		{  0, WHL }, {  0, WHL }, {  0, WHL },
		{  0, HLF },

		{ G4, HLF },
		{  0, WHL }, {  0, WHL }, {  0, WHL },
		{  0, HLF },

		{ G4, HLF },	// final
		{  0, WHL }, {  0, WHL }, {  0, WHL },
		{ A4, HLF },

		{ 0,  0 }
	}
};

const Track bbshark_track_4 = {
	255,
	6,
	&sine,
	{
		{  0, HLF },
		{ B4, WHL },
		{ C5, WHL },
		{ E5, WHL },
		{ D5, HLF },

		{  0, HLF },
		{  0, WHL }, {  0, WHL }, {  0, WHL },
		{  0, HLF },

		{ G4, HLF },
		{  0, WHL }, {  0, WHL }, {  0, WHL },
		{  0, HLF },

		{  0, HLF },
		{  0, WHL }, {  0, WHL }, {  0, WHL },
		{ D4, HLF },

		{ 0,  0 }
	}
};

const Track gravity_track_1 = {
	255,
	6,
	&sine,
	{
		{ F3,  EGH }, { D3,  EGH }, { A2,  EGH }, { D3,  EGH }, { F3,  EGH }, { D3,  EGH }, { A2,  EGH }, { D3,  EGH },
		{ F3,  EGH }, { C3,  EGH }, { A2,  EGH }, { C3,  EGH }, { F3,  EGH }, { C3,  EGH }, { A2,  EGH }, { C3,  EGH },
		{ E3,  EGH }, { D3f, EGH }, { A2,  EGH }, { D3f, EGH }, { E3,  EGH }, { D3f, EGH }, { A2,  EGH }, { D3f, EGH },
		{ E3,  EGH }, { D3f, EGH }, { A2,  EGH }, { D3f, EGH }, { E3,  EGH }, { D3f, EGH }, { A2,  QRT },

		{ D2,  EGH }, { A3,  EGH }, { D3,  EGH }, { A3,  EGH }, { D2,  EGH }, { A3,  EGH }, { D3,  EGH }, { A3,  EGH },
		{ D2,  EGH }, { A3,  EGH }, { D3,  EGH }, { A3,  EGH }, { D2,  EGH }, { A3,  EGH }, { D3,  QRT },
		{ F2,  EGH }, { C3,  EGH }, { D3,  EGH }, { C3,  EGH }, { F2,  EGH }, { C3,  EGH }, { D3,  EGH }, { C3,  EGH },
		{ F2,  EGH }, { C3,  EGH }, { D3,  EGH }, { C3,  EGH }, { F2,  QRT }, { E2,  QRT },

		{ B1f, EGH }, { F2,  EGH }, { B2f, EGH }, { F2,  EGH }, { B1f, EGH }, { F2,  EGH }, { B2f, EGH }, { F2,  EGH },
		{ B1f, EGH }, { F2,  EGH }, { B2f, EGH }, { F2,  EGH }, { B1f, EGH }, { F2,  EGH }, { B2f, QRT },
		{ C2,  EGH }, { G2,  EGH }, { C3,  EGH }, { G2,  EGH }, { C2,  EGH }, { G2,  EGH }, { C3,  QRT },
		{ A2,  EGH }, { E3,  EGH }, { A3,  EGH }, { E3,  EGH }, { A2,  EGH }, { E3,  EGH }, { A3,  QRT },

		{ D2,  EGH }, { A3,  EGH }, { D3,  EGH }, { A3,  EGH }, { D2,  EGH }, { A3,  EGH }, { D3,  QRT },
		{ D2,  EGH }, { A3,  EGH }, { D3,  EGH }, { A3,  EGH }, { D2,  EGH }, { A3,  EGH }, { D3,  QRT },
		{ F2,  EGH }, { C3,  EGH }, { D3,  EGH }, { C3,  EGH }, { F2,  EGH }, { C3,  EGH }, { D3,  QRT },
		{ F2,  EGH }, { C3,  EGH }, { D3,  EGH }, { C3,  EGH }, { F2,  EGH }, { C3,  EGH }, { D3,  QRT },

		{ B1f, EGH }, { F2,  EGH }, { B2f, EGH }, { F2,  EGH }, { B1f, EGH }, { F2,  EGH }, { B2f, QRT },
		{ B1f, EGH }, { F2,  EGH }, { B2f, EGH }, { F2,  EGH }, { B1f, EGH }, { F2,  EGH }, { B2f, QRT },
		{ C2,  EGH }, { G2,  EGH }, { C3,  EGH }, { G2,  EGH }, { C2,  EGH }, { G2,  EGH }, { C3,  QRT },
		{ C2,  EGH }, { G2,  EGH }, { C3,  EGH }, { G2,  EGH }, { C2,  EGH }, { G2,  EGH }, { C3,  QRT },

		{ D2,  EGH }, { A3,  EGH }, { D3,  EGH }, { A3,  EGH }, { D2,  EGH }, { A3,  EGH }, { D3,  QRT },
		{ F2,  EGH }, { C3,  EGH }, { D3,  EGH }, { C3,  EGH }, { F2,  EGH }, { C3,  EGH }, { D3,  QRT },
		{ B1f, EGH }, { F2,  EGH }, { B2f, EGH }, { F2,  EGH }, { B1f, EGH }, { F2,  EGH }, { B2f, QRT },
		{ C2,  EGH }, { G2,  EGH }, { C3,  EGH }, { G2,  EGH }, { C2,  HLF },

		{ A1,  EGH }, { E2,  EGH }, { A2,  EGH }, { E2,  EGH }, { A1,  HLF },
		{ D2,  EGH }, { A2,  EGH }, { D3,  EGH }, { A2,  EGH }, { D2,  HLF },
		{ D2,  QRT }, { 0,   QRT }, { D1,  QRT }, { 0,   QRT },

		{ 0,  0 }
	}
};

const Track gravity_track_2 = {
	255,
	6,
	&sine,
	{
		{ 0,  WHL }, { 0,  WHL }, { 0,  WHL }, { 0,  WHL },
		{ 0,  WHL }, { 0,  WHL }, { 0,  WHL }, { 0,  WHL },
		{ 0,  WHL }, { 0,  WHL }, { 0,  WHL }, { 0,  WHL },
		{ 0,  WHL }, { 0,  WHL }, { 0,  WHL }, { 0,  WHL },
		{ 0,  WHL }, { 0,  WHL }, { 0,  WHL }, { 0,  WHL },
		{ 0,  WHL }, { 0,  WHL }, { 0,  WHL },
		{ 0,  HLF }, { C3, HLF },
		{ 0,  HLF }, { A2, HLF },
		{ 0,  HLF }, { D3, HLF },
		{ 0,  HLF }, { D3, QRT }, { 0, QRT },

		{ 0,  0 }
	}
};

const Track gravity_track_3 = {
	255,
	6,
	&sine,
	{
		{ 0,   WHL  }, { 0,   WHL  }, { 0,   WHL  }, { 0,   WHL  },
		{ D4,  HLFD }, { E4,  QRT },
		{ F4,  WHL  },
		{ A4,  QRTD }, { G4,  QRTD }, { A4,  QRT },
		{ C4,  WHL  },
		{ D4,  HLFD }, { E4,  QRT  },
		{ F4,  HLF  }, { E4,  HLF  },
		{ G4,  HLF  }, { A4,  HLF  },
		{ G4,  HLF  }, { F4,  HLF  },
		{ 0,   QRT  }, { F4,  QRT  }, { F4,  QRT  }, { F4,  QRT  },
		{ A4,  QRT  }, { A4,  QRT  }, { G4,  QRT  }, { F4,  QRT  },
		{ 0,   QRT  }, { A4,  QRT  }, { A4,  QRT  }, { A4,  QRT  },
		{ G4,  QRT  }, { A4,  QRT  }, { G4,  QRT  }, { F4,  QRT  },
		{ 0,   QRT  }, { F4,  QRT  }, { F4,  QRT  }, { F4,  QRT  },
		{ A4,  QRT  }, { A4,  QRT  }, { G4,  QRT  }, { F4,  QRT  },
		{ 0,   QRT  }, { A4,  QRT  }, { A4,  QRT  }, { A4,  QRT  },
		{ 0,   QRT  }, { C5s, QRT  }, { C5s, QRT  }, { C5s, QRT  },
		{ 0,   QRT  }, { F4,  QRT  }, { F4,  QRT  }, { F4,  QRT  },
		{ A4,  QRT  }, { A4,  QRT  }, { G4,  QRT  }, { F4,  QRT  },
		{ 0,   QRT  }, { B4f, QRT  }, { B4f, QRT  }, { B4f, QRT  },
		{ G4,  HLF  }, { C5,  HLF  },
		{ A4,  HLF  }, { C5s, HLF  },
		{ F5,  EGH  }, { D5,  EGH  }, { F5,  EGH  }, { D5,  EGH  }, { A4,  EGH }, { C5s,  EGH }, { E5,  EGH }, { C5s,  EGH },
		{ D5,  QRT  }, { 0,   QRT  }, { D5,  QRT  }, { 0,   QRT  },

		{ 0,  0 }
	}
};

const Track gravity_track_4 = {
	255,
	6,
	&sine,
	{
		{ 0,  WHL }, { 0,  WHL }, { 0,  WHL }, { 0,  WHL },
		{ 0,  WHL }, { 0,  WHL }, { 0,  WHL }, { 0,  WHL },
		{ 0,  WHL }, { 0,  WHL }, { 0,  WHL }, { 0,  WHL },
		{ 0,  WHL }, { 0,  WHL }, { 0,  WHL }, { 0,  WHL },
		{ 0,  WHL }, { 0,  WHL }, { 0,  WHL }, { 0,  WHL },
		{ 0,  WHL }, { 0,  WHL }, { 0,  WHL }, { 0,  WHL },
		{ 0,  WHL }, { 0,  WHL },
		{ 0,  HLF }, { D4, QRT }, { 0, QRT },

		{ 0,  0 }
	}
};

const Song arpeggiator = {
	230,
	{ (PTrack)&arpeg_track_1, (PTrack)&arpeg_track_2, (PTrack)&arpeg_track_3, NULL },
	{ 0, 0, 0, 0 }
};

const Song reveille = {
	340,
	{ (PTrack)&reveille_track_1, (PTrack)&reveille_track_1, (PTrack)&reveille_track_1, NULL },
	{ 0, 3, 9, 0 }
};

const Song bbshark = {
	120,
	{ (PTrack)&bbshark_track_1, (PTrack)&bbshark_track_2, (PTrack)&bbshark_track_3, (PTrack)&bbshark_track_4},
	{ 0, 0, 0, 0 }
};

const Song gravity = {
	165,
	{ (PTrack)&gravity_track_1, (PTrack)&gravity_track_2, (PTrack)&gravity_track_3, (PTrack)&gravity_track_4 },
	{ 0, 0, 0, 0 }
};
