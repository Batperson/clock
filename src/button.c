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

uint8_t	buttonState[16];

void InitButton()
{
	GPIO_InitTypeDef 			gpio;

	memset(buttonState, 0, sizeof(buttonState));

	GPIO_StructInit(&gpio);

	// Pins PA0, PA1, PA9 are input buttons
	gpio.GPIO_Mode				= GPIO_Mode_IPD;
	gpio.GPIO_Speed				= GPIO_Speed_50MHz;
	gpio.GPIO_Pin				= BUTTON_PINS;
	GPIO_Init(BUTTON_PORT, &gpio);
}

// To be called from a repeating interrupt such as SysTick, ideally 1KHz.
void PollButtonState()
{
	uint16_t btn, i;

	for((btn = GPIO_Pin_0, i=0); i < 16; (btn = btn << 1, i++))
	{
		if(btn & (BUTTON_PINS))
		{
			if(btn & BUTTON_PORT->IDR)
			{
				if(buttonState[i] < 0xFF)
				{
					if(buttonState[i]++ == SHORT_PRESS_MS)
						OnButtonDown(btn);
				}
			}
			else if(buttonState[i])
			{
				if(buttonState[i] >= LONG_PRESS_MS)
					OnButtonPress(btn, Long);
				else if(buttonState[i] >= SHORT_PRESS_MS)
					OnButtonPress(btn, Short);

				buttonState[i]	= 0;
			}
		}
	}
}

void WEAKREF OnButtonDown(uint16_t button)
{
	printf("Button down: %d\n", button);
}

void WEAKREF OnButtonPress(uint16_t button, PressType press)
{
	printf("Button press: %d %d\n", button, press);
}
