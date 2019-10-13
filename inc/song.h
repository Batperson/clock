/*
 * song.h
 *
 *  Created on: 3/10/2019
 */

#ifndef SONG_H_
#define SONG_H_

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
	uint8_t legato;		// ticks subtracted from note length to create silences between notes. 0 = very legato, increasing values = staccato
	PWave voice;
	Note notes[];		// note of { 0, 0 } = end of track.
} Track, *PTrack;

typedef struct
{
	uint16_t bpm;		// beats per minute, high value = faster
	Track tracks[];		// track with volume 0 = no more tracks
} Song, *PSong;

void SelectSong(PSong song);
void IncrementTempo(uint16_t inc);
void PlaySong();

#endif /* SONG_H_ */
