/*
 * song.h
 *
 *  Created on: 3/10/2019
 */

#ifndef SONG_H_
#define SONG_H_

// Tick = basic unit of time, 1 quarter note = 48 ticks
#define TICKS_PER_BEAT			((uint32_t)48)

// Note intervals
#define		QRT					TICKS_PER_BEAT		// Quarter note	= crotchet
#define		HLF					QRT * 2				// Half note	= minim
#define		WHL					QRT * 4				// Whole note 	= semibreve
#define		EGH					QRT / 2				// Eighth note 	= quaver
#define 	SXT					QRT / 4				// Sixteenth	= semiquaver

#define		HLFD				HLF + (HLF / 2)		// dotted minim
#define		QRTD				QRT + (QRT / 2)		// dotted crotchet
#define		EGHD				EGH + (EGH / 2)		// dotted quaver

#define		EGHT				QRT / 3				// triplet quaver
#define		EGHX				EGHT * 2			// double triplet quaver

// Note frequencies, Equal Temperament
#define		C1					3270
#define		C1s					3465
#define		D1f					C1s
#define		D1					3671
#define		D1s					3889
#define		E1f					D1s
#define		E1					4120
#define		F1					4365
#define		F1s					4625
#define		G1f					F1s
#define		G1					4900
#define		G1s					5191
#define		A1f					G1s
#define		A1					5500
#define		A1s					5827
#define		B1f					A1s
#define		B1					6174
#define		C2					6541
#define		C2s					6930
#define		D2f					C2s
#define		D2					7342
#define		D2s					7778
#define		E2f					D2s
#define		E2					8241
#define		F2					8731
#define		F2s					9250
#define		G2f					F2s
#define		G2					9800
#define		G2s					10383
#define		A2f					G2s
#define 	A2					11000
#define 	A2s					11654
#define		B2f					A2s
#define 	B2					12347
#define		C3					13081
#define		C3s					13859
#define		D3f					C3s
#define		D3					14683
#define		D3s					15556
#define		E3f					D3s
#define		E3					16481
#define		F3					17461
#define		F3s					18500
#define		G3f					F3s
#define		G3					19600
#define		G3s					20765
#define		A3f					G3s
#define		A3					22000
#define		A3s					22308
#define		B3f					A3s
#define		B3					24694
#define		C4					26163		// Middle C
#define 	C4s					27718
#define		D4f					C4s
#define		D4					29366
#define		D4s					31113
#define		E4f					D4s
#define		E4					32963
#define		F4					34923
#define		F4s					36999
#define		G4f					F4s
#define		G4					39200
#define		G4s					41530
#define		A4f					G4s
#define		A4					44000
#define		A4s					46616
#define		B4f					A4s
#define		B4					49388
#define		C5					52325
#define		C5s					55437
#define		D5f					C5s
#define		D5					58733
#define		D5s					62225
#define		E5f					D5s
#define		E5					65925
#define		F5					69846
#define		F5s					73999
#define		G5f					F5s
#define		G5					78399
#define 	G5s					83061
#define		A5f					G5s
#define		A5					88000
#define		A5s					93233
#define		B5f					A5s
#define		B5					98777
#define		C6					104650
#define		C6s					110873
#define		D6f					C6s
#define		D6					117466
#define		D6s					124451
#define		E6f					D6s
#define		E6					131851
#define		F6					139691
#define		F6s					147998
#define		G6f					F6s
#define		G6					156798
#define		A6					176000

typedef struct
{
	uint32_t pitch;		// Note pitch in hertz * 100. Use defined notes (C4, D4, E4 etc).
	uint16_t interval;	// Number of ticks. Use defined intervals from above (QRT, WHL, EGH etc).
} Note, *PNote;

typedef struct
{
	uint8_t	volume;		// 0 - 255
	uint8_t legato;		// ticks subtracted from note length to create silences between notes. 0 = notes run together, increasing values = staccato
	PVoice voice;		// instrument
	Note notes[];		// note of { 0, 0 } = end of track.
} Track, *PTrack;

typedef struct
{
	uint16_t bpm;							// beats per minute, higher value = faster
	PTrack tracks[SOUND_CHANNELS];			// null = no more tracks. Max = SOUND_CHANNELS
	uint8_t tickOffset[SOUND_CHANNELS];		// ticks to offset the tempo of each track
} Song, *PSong;

typedef enum
{
	PlayNormal	= 0x00,
	PlayLoop	= 0x01
} PlayFlags;

void InitSong();
void SelectSong(PSong song);
void SetTempo(uint16_t bpm);
void IncrementTempo(uint16_t inc);
void PlaySong(PlayFlags flags);
void PauseSong();
void EndSong();

// Events called in interrupt time
void OnSongEnd();
void OnTrackEnd(int track);

#endif /* SONG_H_ */
