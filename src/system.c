/*
 * system.c
 *
 *  Created on: 22/09/2019
 */
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "system.h"

static volatile uint32_t timer;

callback_ptr 	callbacks[MAX_CALLBACKS];
int16_t			timeouts[MAX_CALLBACKS];

void InitSystem()
{
	SysTick_Config(SystemCoreClock / 1000);

	memset(callbacks, 0, sizeof(callbacks));
	memset(timeouts, 0, sizeof(timeouts));

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
			if(timeouts[i] <= 0)
				callbacks[i]();
			if(timeouts[i] == 0)
				DeregisterCallback(callbacks[i]);
			else if(timeouts[i] > 0)
				timeouts[i]--;
		}
	}
}

void RegisterSysTickCallback(callback_ptr ptr)
{
	RegisterTimeoutCallback(ptr, -1);
}

void RegisterTimeoutCallback(callback_ptr ptr, int16_t millis)
{
	for(uint8_t i=0; i<MAX_CALLBACKS; i++)
	{
		if(!callbacks[i])
		{
			callbacks[i] 	= ptr;
			timeouts[i] 	= millis;
			break;
		}
	}
}

void DeregisterCallback(callback_ptr ptr)
{
	for(uint8_t i=0; i<MAX_CALLBACKS; i++)
	{
		if(callbacks[i] == ptr)
		{
			callbacks[i] = NULL;
			break;
		}
	}
}


