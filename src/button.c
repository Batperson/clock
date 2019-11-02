/*
 * button.c
 *
 *  Created on: 28/10/2019
 */

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "macros.h"
#include "button.h"

uint8_t prescaler;
uint8_t	buttonState[16];

void InitButton()
{
	GPIO_InitTypeDef 			gpio;

	memset(buttonState, 0, sizeof(buttonState));
	prescaler 					= 0;

	GPIO_StructInit(&gpio);

	// Pins PA0, PA1, PA9 are input buttons
	gpio.GPIO_Mode				= GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed				= GPIO_Speed_50MHz;
	gpio.GPIO_Pin				= BUTTON_PINS;
	GPIO_Init(BUTTON_PORT, &gpio);

	printf("Buttons initialized\n");
}

// To be called from a repeating interrupt such as SysTick, ideally 1KHz.
void PollButtonState()
{
	if(prescaler++ >= TICK_PRESCALER)
	{
		prescaler = 0;

		uint16_t btn, i;
		for((btn = GPIO_Pin_0, i=0); i < 16; (btn = btn << 1, i++))
		{
			if(btn & (BUTTON_PINS))
			{
				uint8_t bts = buttonState[i];
				if(btn & BUTTON_PORT->IDR)
				{
					if(bts < 0xFF)
					{
						buttonState[i]++;

						if(bts == SHORT_PRESS_TICKS)
							BTN_OnDown(btn, Short);
						else if(bts == LONG_PRESS_TICKS)
							BTN_OnDown(btn, Long);
					}
				}
				else if(bts)
				{
					buttonState[i]	= 0;

					if(bts >= LONG_PRESS_TICKS)
						BTN_OnPress(btn, Long);
					else if(bts >= SHORT_PRESS_TICKS)
						BTN_OnPress(btn, Short);
				}
			}
		}
	}
}

void WEAKREF BTN_OnDown(uint16_t button, PressType press)
{
	printf("Button down: %d %d\n", button, press);
}

void WEAKREF BTN_OnPress(uint16_t button, PressType press)
{
	printf("Button press: %d %d\n", button, press);
}
