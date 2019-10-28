/*
 * system.c
 *
 *  Created on: 22/09/2019
 */
#include <stdio.h>
#include "stm32f10x.h"
#include "button.h"

static volatile uint32_t _millis;
static volatile uint32_t _timer;

void InitSystem()
{
	SysTick_Config(SystemCoreClock / 1000);

	printf("System functions initialized\n");
}

uint32_t millis()
{
	return _millis;
}

void SysTick_Handler(void)
{
	_millis++;
	if(_timer > 0)
		_timer--;

	PollButtonState();
}

void sleep(uint32_t ms)
{
	_timer = ms + 1;
	while(_timer > 0)
		;
}
