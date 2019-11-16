/*
 * system.c
 *
 *  Created on: 22/09/2019
 */
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "system.h"

static volatile uint32_t _millis;
static volatile uint32_t _timer;

callback_ptr 	callbacks[MAX_CALLBACKS];
uint32_t		timeouts[MAX_CALLBACKS];

void InitSystem()
{
	SysTick_Config(SystemCoreClock / 1000);

	memset(callbacks, 0, sizeof(callbacks));
	memset(timeouts, 0, sizeof(timeouts));

	// Set SysTick to a low priority
	NVIC_SetPriority(SysTick_IRQn, 6);

	printf("System functions initialized\n");
}

uint32_t millis()
{
	return _millis;
}

void sleep(uint32_t ms)
{
	_timer = ms + 1;
	while(_timer > 0)
		;
}

void SysTick_Handler(void)
{
	_millis++;
	if(_timer > 0) _timer--;

	for(int i=0; i<MAX_CALLBACKS; i++)
		if(callbacks[i]) callbacks[i]();
}

void RegisterSysTickCallback(callback_ptr ptr)
{
	RegisterTimeoutCallback(ptr, 0);
}

void RegisterTimeoutCallback(callback_ptr ptr, uint32_t millis)
{
	for(uint8_t i=0; i<MAX_CALLBACKS; i++)
	{
		if(!callbacks[i])
		{
			callbacks[i] 	= ptr;
			timeouts[i] 	= 0;
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


