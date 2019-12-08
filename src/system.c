/*
 * system.c
 *
 *  Created on: 22/09/2019
 */
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "system.h"

#define CNT_SHIFT		17
#define CNT_MASK		0x07FE0000
#define RPT_MASK		0x80000000
#define CB_MASK			(~(CNT_MASK | RPT_MASK))

static volatile 		uint32_t timer;
static uint32_t 		callbacks[MAX_CALLBACKS];
static uint16_t			cb_timers[MAX_CALLBACKS];

void InitSystem()
{
	SysTick_Config(SystemCoreClock / 1000);

	memset(callbacks, 0, sizeof(callbacks));

	// Set SysTick to a low priority
	NVIC_SetPriority(SysTick_IRQn, 6);

	printf("System functions initialized\n");
}

void sleep(uint32_t ms)
{
	timer = ms + 1;
	while(timer > 0)
		;
}

void SysTick_Handler(void)
{
	if(timer > 0)
		timer--;

	for(int i=0; i<MAX_CALLBACKS; i++)
	{
		if(callbacks[i])
		{
			if(cb_timers[i] == 0)
			{
				uint32_t val	= callbacks[i];
				callback_ptr cb	= (callback_ptr)(val & CB_MASK);

				cb();

				if(!(val & RPT_MASK))
					callbacks[i] = 0;
				else
					cb_timers[i]	= (val & CNT_MASK) >> CNT_SHIFT;
			}
			else
			{
				cb_timers[i]--;
			}
		}
	}
}

void RegisterTimeoutCallback(callback_ptr ptr, uint16_t millis, CallbackFlags flags)
{
	assert_param(millis <= 0x1F);

	uint32_t val		= (millis << CNT_SHIFT) & CNT_MASK;
	val					|= flags & RPT_MASK;
	val					|= (uint32_t)ptr;

	for(uint8_t i=0; i<MAX_CALLBACKS; i++)
	{
		if(!callbacks[i])
		{
			callbacks[i] 	= val;
			cb_timers[i]	= millis;
			break;
		}
	}
}

void DeregisterCallback(callback_ptr ptr)
{
	for(uint8_t i=0; i<MAX_CALLBACKS; i++)
	{
		if((callbacks[i] & CB_MASK) == (uint32_t)ptr)
		{
			callbacks[i] = 0;
			break;
		}
	}
}


