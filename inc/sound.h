/*
 * sound.h
 *
 *  Created on: 2/10/2019
 */

#ifndef SOUND_H_
#define SOUND_H_

#define WAVE_SAMPLES		256		// 256 exploits 8-bit overflow behaviour of the accumulator
#define SOUND_CHANNELS		4		// This should be a power of 2, so that the mix averaging is a bit-shift instead of a divide operation
#define SOUND_BUFFER_LEN	256		// Could increase or decrease this but make it a factor of 2
#define SAMPLE_RATE			44100	// 44.1KHz sample rate - would be CD quality if we were using 16-bit samples!

// Attack and decay are the number of volume units each sample clock to increment/decrement the volume until
// it reaches either 100% amplitude, or dies away to 0% amplitude.

// Real volume level (as passed to SelectVoice()) is unsigned 8 bits, so 0-255.
// The volume level used for attack and decay is unsigned 16 bits, then right-shifted 8 bits.
// This means that the units for attack and decay are 1/256 of the "real" units.
// So an attack rate of 256 would ramp up volume by 1 "real" unit each sample clock.

typedef struct
{
	uint16_t attack;
	uint16_t decay;
	int8_t sample[WAVE_SAMPLES];
} Voice, *PVoice;

void InitSound();
void SelectVoice(uint8_t channel, uint8_t vol, PVoice pv);
void SoundOn(uint8_t channel, uint32_t hz);
void SoundOff(uint8_t channel);
void Beep(uint32_t hz, uint16_t ms, uint8_t vol);

#endif /* SOUND_H_ */
