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
	uint16_t attack;	// Number of steps each sample clock to increment / decrement the volume offset.
	uint16_t decay;
	uint8_t sample[WAVE_SAMPLES];
} Voice, *PVoice;

void InitSound();
void SelectVoice(uint8_t channel, uint8_t vol, PVoice pv);
void SoundOn(uint8_t channel, uint16_t hz);
void SoundOff(uint8_t channel);


#endif /* SOUND_H_ */
