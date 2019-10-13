/*
 * sound.h
 *
 *  Created on: 2/10/2019
 */

#ifndef SOUND_H_
#define SOUND_H_

#define WAVE_SAMPLES		256
#define SOUND_CHANNELS		4
#define SOUND_BUFFER_LEN	256
#define SAMPLE_RATE			44100

typedef struct
{
	uint8_t sample[WAVE_SAMPLES];
} Wave, *PWave;

void InitSound();
void SelectSound(uint8_t channel, uint8_t vol, PWave pw);
void SoundOn(uint8_t channel, uint16_t hz);
void SoundOff(uint8_t channel);


#endif /* SOUND_H_ */
