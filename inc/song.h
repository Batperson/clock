/*
 * song.h
 *
 *  Created on: 3/10/2019
 */

#ifndef SONG_H_
#define SONG_H_

// Note frequencies, Equal Temperament
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

// Tick = basic unit of time, 1 quarter note = 48 ticks
#define TICKS_PER_BEAT			48

typedef struct
{
	uint8_t pitch;		// Note pitch in hertz * 100
	uint8_t interval;	// 1 = whole note (semibreve), 2 = half note (minim), 4 = quarter (crotchet) etc
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
	uint16_t bpm;		// beats per minute, higher value = faster
	PTrack tracks[];	// null = no more tracks. Max = SOUND_CHANNELS
} Song, *PSong;

void SelectSong(PSong song);
void SetTempo(uint16_t bpm);
void IncrementTempo(uint16_t inc);
void PlaySong();
void PauseSong();

// Events called in interrupt time
void OnSongEnd();
void OnTrackEnd(int track);

#endif /* SONG_H_ */
