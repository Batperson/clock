/*
 * sound.c
 *
 *  Created on: 2/10/2019
 */

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "macros.h"
#include "system.h"
#include "sound.h"
#include "voices.h"

// Set pin A8 as audio output, A2 as amplifier shutdown
#define				PIN_AUDIO	GPIO_Pin_8
#define				PIN_AMPSD	GPIO_Pin_2

typedef enum {
	Sustain		= 0,
	Attack		= 1,
	Decay		= 2
} Phase;

typedef struct
{
	PVoice 				voice;
	uint16_t 			accumulator;
	uint16_t			step;
	uint8_t 			vol;
	Phase				envphase;
	int32_t				envfactor;
} Channel;

volatile Channel channel[SOUND_CHANNELS];

volatile uint16_t soundBuf[SOUND_BUFFER_LEN];

void SoundCallback();

void InitSound()
{
	GPIO_InitTypeDef 			gpio;
	TIM_TimeBaseInitTypeDef		timb;
	TIM_OCInitTypeDef			ocnt;
	DMA_InitTypeDef				dmai;
	NVIC_InitTypeDef 			nvic;

	RegisterTimeoutCallback(&SoundCallback, 0, CallbackRepeat);

	memset((Channel*)channel, 0, sizeof(channel));
	memset((uint16_t*)soundBuf, 0, sizeof(soundBuf));
	for(int i=0; i<SOUND_CHANNELS; i++)
		channel[i].voice = &sine;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// Give the audio DMA interrupt highest priority so it won't be pre-empted causing
	// audible glitches
	nvic.NVIC_IRQChannel 					= DMA1_Channel2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority 	= 0;
	nvic.NVIC_IRQChannelSubPriority 		= 0;
	nvic.NVIC_IRQChannelCmd 				= ENABLE;
	NVIC_Init(&nvic);

	DMA_StructInit(&dmai);

	// DMA target is the TIM1 Capture & Compare Register 1
	// Mode is circular, restart from the beginning when transfer complete
	// Interrupts on half transfer and complete transfer
	dmai.DMA_M2M					= DMA_M2M_Disable;
	dmai.DMA_MemoryBaseAddr 		= (uint32_t)soundBuf;
	dmai.DMA_BufferSize 			= SOUND_BUFFER_LEN;
	dmai.DMA_PeripheralBaseAddr 	= (uint32_t)&TIM1->CCR1;
	dmai.DMA_DIR 					= DMA_DIR_PeripheralDST;
	dmai.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;
	dmai.DMA_MemoryInc 				= DMA_MemoryInc_Enable;
	dmai.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_HalfWord;
	dmai.DMA_MemoryDataSize 		= DMA_MemoryDataSize_HalfWord;
	dmai.DMA_Mode 					= DMA_Mode_Circular;
	dmai.DMA_Priority 				= DMA_Priority_High;

	DMA_Init(DMA1_Channel2, &dmai);
	DMA_ITConfig(DMA1_Channel2, DMA_IT_HT | DMA_IT_TC, ENABLE);
	DMA_ClearITPendingBit(DMA1_IT_TC2 | DMA1_IT_HT2 | DMA1_IT_TE2 | DMA1_IT_GL2);

	DMA_Cmd(DMA1_Channel2, ENABLE);

	GPIO_ResetBits(GPIOA, PIN_AMPSD);
	GPIO_StructInit(&gpio);

	// Use pin for TIM1 OC1
	gpio.GPIO_Mode				= GPIO_Mode_AF_PP;
	gpio.GPIO_Speed				= GPIO_Speed_50MHz;
	gpio.GPIO_Pin				= PIN_AUDIO;
	GPIO_Init(GPIOA, &gpio);

	// Use pin for amplifier shutdown
	gpio.GPIO_Mode				= GPIO_Mode_Out_PP;
	gpio.GPIO_Speed				= GPIO_Speed_2MHz;
	gpio.GPIO_Pin				= PIN_AMPSD;

	GPIO_Init(GPIOA, &gpio);

	// TIM1 OC1 will be the analog output
	// TIM2 will be the sample clock, driving DMA to set TIM1 C1's OC.

	TIM_TimeBaseStructInit(&timb);
	TIM_OCStructInit(&ocnt);

	// Using 8-bit sampling so output range is 0 to 255.
	timb.TIM_Prescaler 			= 0;
	timb.TIM_CounterMode 		= TIM_CounterMode_Up;
	timb.TIM_Period 			= 255;
	timb.TIM_ClockDivision 		= TIM_CKD_DIV1;
	timb.TIM_RepetitionCounter 	= 0;
	TIM_TimeBaseInit(TIM1, &timb);

	ocnt.TIM_OCMode 			= TIM_OCMode_PWM1;
	ocnt.TIM_Pulse 				= 127;
	ocnt.TIM_OutputState 		= TIM_OutputState_Enable;
	ocnt.TIM_OCPolarity 		= TIM_OCPolarity_High;
	ocnt.TIM_OCIdleState 		= TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM1, &ocnt);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);

	TIM_TimeBaseStructInit(&timb);

	// Note: TIM_Period is not correct if the timer is clocked at half system clock.
	// But sound appears to be 2* correct frequency if I don't do this.
	timb.TIM_Prescaler 			= 0;
	timb.TIM_CounterMode 		= TIM_CounterMode_Up;
	timb.TIM_Period 			= (((SystemCoreClock /* / 2 */) / SAMPLE_RATE) - 1);	// 44.1KHz sample rate
	timb.TIM_ClockDivision 		= TIM_CKD_DIV1;
	timb.TIM_RepetitionCounter 	= 0;
	TIM_TimeBaseInit(TIM2, &timb);

	TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	printf("Sound initialized\n");
}

void AudioOn()
{
	//if(++ampRefCount > 0)
		GPIO_SetBits(GPIOA, PIN_AMPSD);
}

void AudioOff()
{
	//if(--ampRefCount == 0)
		GPIO_ResetBits(GPIOA, PIN_AMPSD);
}

void LoadSoundBuffer(uint16_t bufhalf)
{
	uint16_t start	= bufhalf * (SOUND_BUFFER_LEN / 2);
	uint16_t end	= (bufhalf + 1) * (SOUND_BUFFER_LEN / 2);

	for(uint16_t i = start; i < end; i++)
	{
		int32_t val	= 0;
		for(int j=0; j<4; j++)
		{
			// If we're not stepping through samples, we're not playing
			if(channel[j].step > 0)
			{
				int32_t rval = 		channel[j].voice->sample[channel[j].accumulator >> 8];
				rval *=				channel[j].vol;

				channel[j].accumulator += channel[j].step;

				switch(channel[j].envphase)
				{
				case Attack:
					rval *=			channel[j].envfactor >> 8;
					val += 			rval >> 16;

					if((channel[j].envfactor += channel[j].voice->attack) >= 0xffff)
						channel[j].envphase		= Sustain;
					break;

				case Decay:
					rval *=			channel[j].envfactor >> 8;
					val += 			rval >> 16;

					if((channel[j].envfactor -= channel[j].voice->decay) <= 0)
						channel[j].step			= 0;	// Stop playing
					break;

				default:
					val += 			rval >> 8;
					break;
				}
			}
		}

		val /= SOUND_CHANNELS;

		soundBuf[i]	= (uint16_t)(127 + val);
	}
}

void INTERRUPT DMA1_Channel2_IRQHandler()
{
	uint32_t sr			= DMA1->ISR & (DMA_ISR_TCIF2 | DMA_ISR_HTIF2);
	DMA1->IFCR			= DMA_IFCR_CGIF2 | DMA_IFCR_CTCIF2 | DMA_IFCR_CHTIF2 | DMA_IFCR_CTEIF2;

	LoadSoundBuffer((sr & DMA_ISR_TCIF2) ? 1 : 0);
}

void SelectVoice(uint8_t chan, uint8_t vol, PVoice voice)
{
	channel[chan].voice			= voice;
	channel[chan].vol			= vol;
}

void SoundOn(uint8_t chan, uint32_t hz)
{
	// float s = ((float)WAVE_SAMPLES * 256) / ((float)SAMPLE_RATE / (float)hz);
	// float m = (SAMPLE_RATE / 1000) * ms;

	// Note hz = hertz * 100
	channel[chan].accumulator	= 0;
	channel[chan].step			= ((uint32_t)(((WAVE_SAMPLES * 256) << 8) / ((SAMPLE_RATE * 100) / hz))) >> 8;
	channel[chan].envphase		= Attack;
	channel[chan].envfactor		= 0;
}

void SoundOff(uint8_t chan)
{
	channel[chan].envphase		= Decay;
	channel[chan].envfactor		= 0xffff;
}

#define BEEP_CHANNEL SOUND_CHANNELS-1

uint8_t	oldVol		= 0;
PVoice oldVoice		= 0;
uint16_t beepTicks 	= 0;

void Beep(uint32_t hz, uint16_t ms, uint8_t vol)
{
	if(ms > 0)
	{
		oldVoice	= channel[BEEP_CHANNEL].voice;
		oldVol		= channel[BEEP_CHANNEL].vol;

		SelectVoice(BEEP_CHANNEL, vol, &sine);
		SoundOn(BEEP_CHANNEL, hz);

		beepTicks = ms;
	}
}

void SoundCallback()
{
	if(beepTicks)
	{
		if(--beepTicks == 0)
		{
			SoundOff(BEEP_CHANNEL);

			if(oldVoice)
				SelectVoice(BEEP_CHANNEL, oldVol, oldVoice);
		}
	}
}
