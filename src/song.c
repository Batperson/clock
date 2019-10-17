/*
 * song.c
 *
 *  Created on: 13/10/2019
 */

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "macros.h"
#include "system.h"
#include "sound.h"
#include "song.h"

#define INTERVAL_TO_TICKS(intv) (uint16_t)((TICKS_PER_BEAT * 4) / intv)

typedef enum
{
	None		= 0x00,
	Active		= 0x01,
} TrackFlags;

typedef struct
{
	TrackFlags	flags;
	uint16_t 	note;
	uint16_t	ticks;
} TrackState, *PTrackState;

typedef struct
{
	PSong		song;
	uint16_t 	tempo;
	TrackState track[SOUND_CHANNELS];
} SongState;

volatile SongState state;

void InitSong()
{
	TIM_TimeBaseInitTypeDef		timb;
	NVIC_InitTypeDef 			nvic;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructInit(&timb);

	timb.TIM_Prescaler 			= 0;
	timb.TIM_CounterMode 		= TIM_CounterMode_Up;
	timb.TIM_Period 			= 0;
	timb.TIM_ClockDivision 		= TIM_CKD_DIV1;
	timb.TIM_RepetitionCounter 	= 0;
	TIM_TimeBaseInit(TIM2, &timb);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	nvic.NVIC_IRQChannel 					= TIM3_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority 	= 0;
	nvic.NVIC_IRQChannelSubPriority 		= 0;
	nvic.NVIC_IRQChannelCmd 				= ENABLE;
	NVIC_Init(&nvic);
}

void SetTempo(uint16_t bpm)
{
	state.tempo = bpm;

	TIM_SetAutoreload(TIM3, ((SystemCoreClock / 2) / (60 * bpm)) - 1);
}

void IncrementTempo(uint16_t bpm)
{
	SetTempo(state.tempo + bpm);
}

void InitTracks()
{
	for(int i=0; i<SOUND_CHANNELS; i++)
	{
		SoundOff(i);
		memset((PTrackState)&state.track[i], 0, sizeof(TrackState));
	}

	for(int i=0; i<SOUND_CHANNELS; i++)
	{
		PTrackState ts 	= (PTrackState)&state.track[i];
		PTrack t		= state.song->tracks[i];

		if(t == NULL)
			break;

		ts->flags |= Active;
		SelectVoice(i, t->volume, t->voice);
	}
}

void SelectSong(PSong song)
{
	state.song = song;

	SetTempo(song->bpm);

	InitTracks();
}

void PlaySong()
{
	TIM_Cmd(TIM3, ENABLE);
}

void PauseSong()
{
	TIM_Cmd(TIM3, DISABLE);

	for(int i=0; i< SOUND_CHANNELS; i++)
		SoundOff(i);
}

void WEAKREF OnSongEnd()
{

}

void WEAKREF OnTrackEnd(int track)
{

}

void EndSong()
{
	TIM_Cmd(TIM3, DISABLE);
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	InitTracks();

	OnSongEnd();
}

void INTERRUPT TIM3_IRQHandler()
{
	TIM3->SR		&= ~TIM_SR_UIF;

	uint8_t atracks	= 0;

	for(int i=0; i < SOUND_CHANNELS; i++)
	{
		PTrackState ts 	= (PTrackState)&state.track[i];
		PTrack t		= state.song->tracks[i];

		if(ts->flags & Active)
		{
			atracks++;
			if(ts->ticks-- == 0)
			{
				PNote n		= &t->notes[ts->note++];
				if(n->interval == 0 && n->pitch == 0)
				{
					// Track ends
					ts->flags			= None;
					atracks--;

					OnTrackEnd(i);
				}
				else
				{
					ts->ticks			= INTERVAL_TO_TICKS(n->interval);

					if(n->pitch > 0)
						SoundOn(i, n->pitch);
					else
						SoundOff(i);
				}
			}
			else if(ts->ticks == t->legato)
			{
				SoundOff(i);
			}
		}
	}

	if(atracks == 0)
		EndSong();
}
