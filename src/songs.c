/*
 * songs.c
 *
 *  Created on: 14/10/2019
 */

#include <stddef.h>
#include "stm32f10x.h"
#include "sound.h"
#include "song.h"
#include "voices.h"

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
	&clipped,
	{
		{ C5s, WHL + WHL  },
		{ D5,  WHL + WHL  },
		{ C5s, WHL + WHL  },
		{ D5,  WHL + WHL  },
		{ C5s, WHL + WHL  },
		{ B4,  WHL + WHL  },
		{ A4,  WHL + WHL  },
		{ E4,  WHL  },
		{ A3,  HLF  },
		{ 0, 0 }
	}
};

const Track arpeg_track_3 = {
	255,
	0,
	&clipped,
	{
		{ A4,  WHL + WHL  },
		{ A4,  WHL + WHL  },
		{ A4,  WHL + WHL  },
		{ A4,  WHL + WHL  },
		{ A4,  WHL + WHL  },
		{ B4,  WHL + WHL  },
		{ A4,  WHL + WHL  },
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
	&clipped,
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
	&clipped,
	{
		{  0, HLF },
		{ G4, WHL },
		{ G4, WHL },
		{ C5, WHL },
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
	&clipped,
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
	&saw,
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
	&saw,
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
	&saw,
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
	&saw,
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

const Track birthday_track_1 = {
	255,
	6,
	&saw,
	{
	                   	   	   	   	  { G3,  EGHD }, { G3,  SXT  },
		{ A3,  QRT  }, { G3,  QRT  }, { C4,  QRT  },
		{ B3,  HLF  }, { G3,  EGHD }, { G3,  SXT  },
		{ A3,  QRT  }, { G3,  QRT  }, { D4,  QRT  },
		{ C4,  HLF  }, { G3,  EGHD }, { G3,  SXT  },
		{ G4,  QRT  }, { E4,  QRT  }, { C4,  QRT  },
		{ B3,  QRT  }, { A3,  QRT  }, { F4,  EGHD }, { F4,  SXT  },
		{ E4,  QRT  }, { C4,  QRT  }, { D4,  QRT  },
		{ C4,  QRT  }, { 0,   HLF  },

		{ 0,  0 }
	}
};

const Track birthday_track_2 = {
	255,
	6,
	&sine,
	{
		{ 0,   QRT  },
		{ 0,   HLFD },
		{ G3,  HLFD },
		{ 0,   HLFD },
		{ C4,  HLFD },
		{ C4,  HLF  }, { C4,  HLF }, { F4,  HLF  },
		{ C4,  HLF  }, { G3,  QRT },
		{ C4,  QRT  },

		{ 0,  0 }
	}
};

const Track birthday_track_3 = {
	255,
	6,
	&sine,
	{
		{ 0,   QRT  },
		{ 0,   HLFD },
		{ B3,  HLFD },
		{ 0,   HLFD },
		{ E4,  HLFD },
		{ E4,  HLF  }, { B4f,  HLF }, { A4,  HLF  },
		{ E4,  HLF  }, { B3,  QRT },
		{ E4,  QRT  },

		{ 0,  0 }
	}
};


const Track birthday_track_4 = {
	255,
	6,
	&sine,
	{
		{ 0,   QRT  },
		{ 0,   HLFD },
		{ D4,  HLFD },
		{ 0,   HLFD },
		{ G4,  HLFD },
		{ G4,  HLF  }, { G4,  HLF }, { C5,  HLF  },
		{ G4,  HLF  }, { D4,  QRT },
		{ G4,  QRT  },

		{ 0,  0 }
	}
};

const Track anthem_track_1 = {
	255,
	6,
	&saw,
	{
		{ C4,  QRT  }, { B3,  QRT  }, { C4,  QRT  }, { G3,  QRT  },
		{ E4,  QRT  }, { E4,  EGH  }, { D4,  EGH  }, { C4,  HLF  },
		{ A3,  QRT  }, { F4,  QRT  }, { G3,  QRT  }, { E4,  QRT  },
		{ D4,  EGH  }, { C4,  EGH  }, { B3,  EGH  }, { A3,  EGH  }, { G3,  HLF },
		{ C4,  QRT  }, { G3,  QRT  }, { A3,  QRT  }, { B3,  QRT  },
		{ C4,  QRT  }, { D4,  QRT  }, { E4,  HLF  },
		{ E4,  QRT  }, { C4,  QRT  }, { A3,  QRT  }, { D4,  QRT  },
		{ C4,  QRT  }, { B3,  QRT  }, { C4,  HLF  },

		{ G4,  QRT  }, { G4,  QRT  }, { C4,  QRT  }, { E4,  QRT  },
		{ G4,  QRT  }, { G4,  EGH  }, { A4,  EGH  }, { G4,  HLF  },
		{ D4,  QRT  }, { E4,  QRT  }, { F4,  EGH  }, { E4,  EGH  }, { D4,  EGH  }, { C4,  EGH },
		{ B3,  EGH  }, { C4,  EGH  }, { B3,  EGH  }, { A3,  EGH  }, { G3,  HLF  },
		{ C4,  QRT  }, { G3,  QRT  }, { A3,  QRT  }, { B3,  QRT  },
		{ C4,  QRT  }, { D4,  QRT  }, { G4,  HLF  },

		{ G4,  QRT  }, { F4,  QRT  }, { E4,  QRT  }, { F4,  QRT  },
		{ E4,  HLF  }, { D4,  HLF  },
		{ C4,  EGH  }, { C4,  EGH + QRT },

		{ 0,   0    }
	}
};

const Track anthem_track_2 = {
	180,
	6,
	&clipped,
	{
		{ C3,  WHL  }, { C3,  WHL  }, { F3,  HLF  }, { E3,  HLF  }, { D3,  HLF  }, { G3,  EGH  }, { F3,  EGH  }, { E3,  EGH  }, { D3,  EGH  },
		{ C3,  WHL  }, { C3,  HLF  }, { E3,  HLF  }, { C3,  HLF  }, { F3,  HLF  }, { G3,  HLF  }, { C4,  HLF  },

		{ C3,  WHL  }, { E3,  WHL  }, { A2,  HLF  }, { F3,  HLF  }, { D3,  HLF  }, { G3,  EGH  }, { F3,  EGH  }, { E3,  EGH  }, { D3,  EGH  },
		{ C3,  WHL  }, { C3,  HLF  }, { G3,  HLF  }, { E3,  HLF  }, { F3,  HLF  }, { C3,  HLF  }, { G3,  HLF  }, { C3,  HLF  },

		{ 0,   0    }
	}
};

const Track anthem_track_3 = {
	180,
	6,
	&clipped,
	{
		{ E3,  WHL  }, { E3,  WHL  }, { A3,  HLF  }, { G3,  HLF  }, { F3s, HLF  }, { B3,  HLF },
		{ E3,  WHL  }, { E3,  HLF  }, { G3,  HLF  }, { B2f, HLF  }, { A3,  HLF  }, { B3,  HLF  }, { E3,  HLF },

		{ E3,  WHL  }, { G3,  WHL  }, { C3,  HLF  }, { A3,  HLF  }, { F3s, HLF  }, { B3,  HLF },
		{ E3,  WHL  }, { E3,  HLF  }, { B3,  HLF  }, { G3,  HLF  }, { A3,  HLF  }, { E3,  HLF  }, { B3,  HLF }, { E3,  HLF },

		{ 0,   0    }
	}
};

const Track anthem_track_4 = {
	180,
	6,
	&clipped,
	{
		{ G3,  WHL  }, { G3,  WHL  }, { C4,  HLF  }, { E3,  HLF  }, { A3,  HLF  }, { D4,  HLF  },
		{ G3,  WHL  }, { G3,  HLF  }, { B3,  HLF  }, { G3,  HLF  }, { F3,  HLF  }, { D4,  HLF  }, { C4,  HLF  },

		{ G3,  WHL  }, { B3,  WHL  }, { E3,  HLF  }, { C4,  HLF  }, { A3,  HLF  }, { D4,  HLF  },
		{ G3,  WHL  }, { G3,  HLF  }, { D4,  HLF  }, { B3,  HLF  }, { C4,  HLF  }, { G3,  HLF  }, { G3,  HLF  }, { D4,  HLF },

		{ 0,   0    }
	}
};

const Track starwars_track_1 = {
	255,
	6,
	&saw,
	{
		{ 0,    QRT },
		{ G4,   HLF }, { D5,   HLF },
		{ C5,  EGHT }, { B4,  EGHT }, { A4,  EGHT }, { G5,   HLF }, { D5,   QRT },
		{ C5,  EGHT }, { B4,  EGHT }, { A4,  EGHT }, { G5,   HLF }, { D5,   QRT },
		{ C5,  EGHT }, { B4,  EGHT }, { C5,  EGHT }, { A4,   HLF }, { 0,    QRT },

		{ G4,   HLF }, { D5,   HLF },
		{ C5,  EGHT }, { B4,  EGHT }, { A4,  EGHT }, { G5,   HLF }, { D5,  QRT },
		{ C5,  EGHT }, { B4,  EGHT }, { A4,  EGHT }, { G5,   HLF }, { D5,  QRT },
		{ C5,  EGHT }, { B4,  EGHT }, { C5,  EGHT }, { A4,   HLF }, { 0,   QRT },

		{ E4,  QRTD }, { E4,   EGH }, { C5,   EGH }, { B4,   EGH }, { A4,  EGH }, { G4,   EGH },
		{ G4,  EGHT }, { A4,  EGHT }, { B4,  EGHT }, { A4,  EGHD }, { E4,  SXT }, { F4s,  QRT }, { 0,   QRT },
		{ E4,  QRTD }, { E4,   EGH }, { C5,   EGH }, { B4,   EGH }, { A4,  EGH }, { G4,   EGH },
		{ D5,   QRT }, { A4,   HLF }, { 0,    QRT },

		{ E4,  QRTD }, { E4,   EGH }, { C5,   EGH }, { B4,   EGH }, { A4, EGHD }, { G4,   SXT },
		{ G4,  EGHT }, { A4,  EGHT }, { B4,  EGHT }, { A4,  EGHD }, { E4,  SXT }, { F4s,  QRT }, { D5,  EGHD }, { D5,   SXT },
		{ G5,   EGH }, { F5,   EGH }, { E5f, EGHD }, { D5,   SXT }, { C5,  EGH }, { B4f,  EGH }, { A4,  EGHD }, { G4,   SXT },
		{ D5,  HLFD }, /*{ 0,  QRT },*/


		{ 0,   0    }
	}
};

const Track starwars_track_2 = {
	255,
	6,
	&saw,
	{
		{ D4,  EGHT }, { D4,  EGHT }, { D4,  EGHT },
		{ G3,   QRT }, { G3,   HLF }, { G3,   QRT },
		{ G3,   HLF }, { G3,   HLF },
		{ G3,   HLF }, { G3,   HLF },
		{ F4,   QRT }, { F4s,  HLF }, { D4,  EGHT }, { D4,  EGHT }, { D4,  EGHT },

		{ G3,   QRT }, { G3,   HLF }, { G3,   QRT },
		{ G3,   HLF }, { G3,   HLF },
		{ G3,   HLF }, { G3,   HLF },
		{ F4,   QRT }, { F4s,  HLF }, { D4,   EGHD }, { D4,   SXT },

		{ G3,   QRT }, { G3,   HLF }, { G3,   QRT },
		{ G3,   QRT }, { A4,   HLF }, { D4,   EGHD }, { D4,   SXT },
		{ G3,   QRT }, { G3,   HLF }, { G3,   QRT },
		{ G3,   QRT }, { A4,   HLF }, { D4,   EGHD }, { D4,   SXT },

		{ G3,   QRT }, { G3,   HLF }, { G3,    QRT },
		{ G3,   QRT }, { A4,   HLF }, { 0,     QRT },
		{ E3f,  HLF }, { G4,   HLF },
		{ D4,   QRT }, { D4,  EGHT }, { D4,  EGHT }, { D4,  EGHT }, { D4,   QRT }, /* { D4,  EGHT }, { D4,  EGHT }, { D4,  EGHT }, */

		{ 0,   0    }
	}
};

const Track starwars_track_3 = {
	255,
	6,
	&saw,
	{
		{ 0,    QRT },
		{ B3,   QRT }, { B3,   HLF }, { B3,   QRT },
		{ C4,   HLF }, { B3,   HLF },
		{ C4,   HLF }, { B3,   HLF },
		{ A3,   QRT }, { A3,   HLF }, { 0,    QRT },

		{ B3,   QRT }, { B3,   HLF }, { B3,   QRT },
		{ C4,   HLF }, { B3,   HLF },
		{ C4,   HLF }, { B3,   HLF },
		{ A3,   QRT }, { A3,   HLF }, { 0,    QRT },

		{ A3,   QRT }, { A3,   HLF }, { A3,   QRT },
		{ A3,   QRT }, { A3,   HLF }, { 0,    QRT },
		{ A3,   QRT }, { A3,   HLF }, { A3,   QRT },
		{ D4,   QRT }, { D4,   HLF }, { 0,    QRT },

		{ A3,   QRT }, { A3,   HLF }, { A3,   QRT },
		{ A3,   HLF }, { D4,   QRT }, { 0,    QRT },
		{ G3,   HLF }, { C4,   HLF },
		{ F4s,  HLF }, {  0,   QRT /*HLF*/ },




		{ 0,   0    }
	}
};

const Track starwars_track_4 = {
	255,
	6,
	&saw,
	{
		{ 0,    QRT },
		{ D4,   QRT }, { D4,   HLF }, { D4,   QRT },
		{ E4,   HLF }, { D4,   HLF },
		{ E4,   HLF }, { D4,   HLF },
		{ C4,   QRT }, { D4,   EGH }, { E5,   SXT }, { F5s,  SXT }, { G5,   SXT }, { A5,   SXT }, { B5,   SXT }, { C6,   SXT }, { D6,   SXTT }, { E6,  SXTT }, { F6s, SXTT },

		{ G6,   EGH }, { G6,   SXT }, { F6s,  SXT }, { G6,   EGH }, { G6,   SXT }, { F6s,  SXT }, { G6,   EGH }, { G6,   SXT }, { F6s,   SXT }, { G6,   EGH }, { G6,   SXT }, { F6s,   SXT },
		{ G6,   EGH }, { G6,   SXT }, { F6s,  SXT }, { G6,   EGH }, { G6,   SXT }, { F6s,  SXT }, { G6,   EGH }, { G6,   SXT }, { F6s,   SXT }, { G6,   EGH }, { G6,   SXT }, { F6s,   SXT },
		{ G6,   EGH }, { G6,   SXT }, { F6s,  SXT }, { G6,   EGH }, { G6,   SXT }, { F6s,  SXT }, { G6,   EGH }, { G6,   SXT }, { F6s,   SXT }, { G6,   EGH }, { G6,   SXT }, { F6s,   SXT },
		{ G6,   EGH }, { A6,   SXT }, { G6,   SXT }, { 0,    EGH }, { F6s,  SXT }, { G6,   SXT }, { A6,   QRT }, { 0,    QRT },

		{ 0,    WHL * 4 },

		{ 0,    WHL * 3 },
		{ A4,   HLF }, {  0,   QRT /*HLF*/ },

		{ 0,    0   }
	}
};

const Track unknown_track_1 = {
	255,
	6,
	&clipped,
	{
		{ 0,    EGH },
		{ 0,   QRTD }, { G4f,  QRT }, { F4,   HLF }, { G4f,  EGH }, { F4,   EGH }, { E4f,  EGH
			 + WHL + HLF }, // oh oh
		{ 0,   QRTD }, { G4f,  QRT }, { F4,   HLF }, { G4f,  EGH }, { F4,   EGH }, { E4f,  EGH
			 + QRTD }, { 0, QRT + EGH }, /*{ D4f,  QRT }, { C4,   EGH },*/  { E4f,  EGH }, { E4f,  EGH }, { E4f,  EGH }, { B3f, QRT }, { B4f, EGH // into the unknown
             + WHL + HLF

             + QRTD }, { 0,   QRTD }, { E4f,  EGH }, { E4f,  EGH }, { E4f,  EGH }, { B3f, QRT }, { C5, EGH // into the unknown
             + QRT  }, { B4f, WHL + QRT + QRTD }, { E4f,  EGH }, { E4f,  EGH }, { E4f,  EGH }, { B3f, QRT }, { E5f, EGH // into the unknown
             + QRTD + QRT }, { D5,   HLF }, { E5f,  QRT }, { C5, EGH
             + WHL + HLF },
		 { 0,  QRTD },  { G4f,   QRT }, { F4,   HLF }, { G4f,  EGH }, { F4,   EGH }, { E4f,  EGH // oh oh
			 + HLFD },  { 0,     HLF }, { E4f,  EGH }, { E4f,  EGH }, // I can
		 { F4,   QRT }, { E4f,   HLF }, { 0,    HLF }, { C4,   EGH }, { E4f,  EGH }, // hear you but I
		 { E4f,  QRT }, { 0,    HLFD }, { G4f, QRTD }, { F4,   EGH }, { E4f,  EGH }, // won't, some look for
		 { F4,   EGH }, { E4f,   QRT }, { 0, HLF + EGH }, { C4,  QRT }, { D4, EGH }, { E4f,  EGH }, //trouble while others
		 { C4,   QRT }, { 0,   HLFD + HLF }, { E4f,  EGH }, { E4f,  EGH }, // don't. There's a
		 { B4f, EGHP }, { G4f, EGHP }, { F4,  EGHP }, { E4f,  EGHP + QRTD }, {0,   EGH }, { F4,   EGH }, { E4f,  EGH }, // thousand reasons I should
		 { F4,   EGH }, { E4f,  EGH }, { F4,   EGH }, { G4f,  QRT }, { E4f,  HLF }, { 0,    EGH }, { E4f,  EGH }, { E4f,  EGH }, // go about my day and ig-
		 { B4f, EGHP }, { G4f, EGHP }, { F4,  EGHP }, { E4f,  EGHP + QRTD }, {0,   EGH }, { F4,   EGH }, { E4f,  EGH }, // nore your whispers which I
		 { F4,   QRT }, { E4f,  EGH }, { F4,   EGH }, { G4f,  EGH }, { A4f, EGH + QRTD + EGH }, { G4f,  EGH }, { F4,   EGH }, // wish would go away oh oh
		 { E4f,   EGH }, { 0,    EGH + HLF + HLFD }, // oh

		{ 0,    0   }
	}
};

const Track unknown_track_2 = {
	255,
	6,
	&saw,
	{
		{ G2,   EGH },
		{ A2f,  (QRT * 9) + QRTD }, { 0,    QRT }, { G2,   EGH },
		{ A2f,  (QRT * 12) },
		{ E2f,  (QRT * 12) },
		{ A1f,  (QRT * 12) },
		{ C2,   QRT }, { C2,   EGH }, { 0,    EGH }, { C2,   EGH }, { C2,   EGH }, { C2,   QRT }, { C2,   EGH }, { 0,    EGH }, { C2,   EGH }, { C2,   EGH },
		{ C2,   QRT }, { C2,   EGH }, { 0,    EGH }, { C2,   EGH }, { C2,   EGH }, { C2,   QRT }, { C2,   EGH }, { 0,    EGH }, { C2,   EGH }, { C2,   EGH },
		{ A1f,  (QRT * 12) },
		{ E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH },
		{ E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH },
		{ E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH },
		{ E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH },
		{ E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH },
		{ E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH },
		{ E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH },
		{ E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH }, { E3f,  QRT }, { E3f,  EGH },
		{ E2f, QRTD }, { E2f, QRTD }, { E2f, QRTD }, { E2f, QRTD },

		{ 0,    0   }
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

const Song birthday = {
	140,
	{ (PTrack)&birthday_track_1, (PTrack)&birthday_track_2, (PTrack)&birthday_track_3, (PTrack)&birthday_track_4 },
	{ 0, 0, 0, 0 }
};

const Song anthem = {
	125,
	{ (PTrack)&anthem_track_1, (PTrack)&anthem_track_2, (PTrack)&anthem_track_3, (PTrack)&anthem_track_4 },
	{ 0, 0, 0, 0 }
};

const Song starwars = {
	125,
	{ (PTrack)&starwars_track_1, (PTrack)&starwars_track_2, (PTrack)&starwars_track_3, (PTrack)&starwars_track_4 },
	{ 0, 0, 0, 0 }
};

const Song unknown = {
	170,
	{ (PTrack)&unknown_track_1, (PTrack)&unknown_track_2, (PTrack)NULL, (PTrack)NULL, },
	{ 0, 0, 0, 0 }
};
