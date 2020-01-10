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
		{ D4, WHL },
		{ F3s, HLF },

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
		{ G4, WHL },
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
		{ B4, WHL },
		{ D4, HLF },

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
	160,
	{ (PTrack)&bbshark_track_1, (PTrack)&bbshark_track_2, (PTrack)&bbshark_track_3, (PTrack)&bbshark_track_4},
	{ 0, 0, 0, 0 }
};
