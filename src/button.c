/*
 * button.c
 *
 *  Created on: 28/10/2019
 */

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "macros.h"
#include "system.h"
#include "button.h"

static uint32_t			state[BUTTON_MAX_CBACK];
static uint32_t			callback[BUTTON_MAX_CBACK];

#define	CALLBACK_MASK	0x0801FFFF
#define CNT_MASK		0x07F80000
#define EVENT_MASK		0xF0000000

#define EVENT_SHIFT		28
#define CNT_SHIFT		19

void WEAKREF OnButtonEvent(uint32_t button, ButtonEventType eventType)
{

}

// To be called from a repeating interrupt such as SysTick, ideally 1KHz.
void PollButtonState()
{
	for(uint8_t i=0; i<BUTTON_MAX_CBACK; i++)
	{
		if(state[i] == 0)
			break;

		uint16_t btn			= state[i] & 0xFFFF;
		uint8_t cnt				= (callback[i] & CNT_MASK) >> CNT_SHIFT;
		ButtonEventType eta 	= ButtonNone;

		if(BUTTON_PORT->IDR & btn)
		{
			if(cnt < 0xFF)
			{
				state[i] &= 0x0000FFFF;
				state[i] |= ((BUTTON_PORT->IDR & btn) << 16);

				cnt++;

				if(cnt == SHORT_PRESS_TICKS)
					eta = ButtonShortDown;
				else if(cnt == LONG_PRESS_TICKS)
					eta = ButtonLongDown;
			}
		}
		else
		{
			if(cnt >= LONG_PRESS_TICKS)
				eta = ButtonLongPress;
			else if(cnt >= SHORT_PRESS_TICKS)
				eta = ButtonShortPress;

			cnt = 0;
		}

		callback[i]	&= ~CNT_MASK;
		callback[i] |= cnt << CNT_SHIFT;

		if(eta != ButtonNone)
		{
			uint16_t btna		= (state[i] & 0xFFFF0000) >> 16;
			ButtonEventType et	= (callback[i] & EVENT_MASK) >> EVENT_SHIFT;
			button_callback cbk	= (button_callback)(callback[i] & CALLBACK_MASK);

			printf("btn %i %d %d\n", i, btna, eta);

			OnButtonEvent(btna, eta);

			if((eta & et) == eta)
				cbk(btna, eta);
		}
	}
}

void InitButton()
{
	GPIO_InitTypeDef 			gpio;

	memset(state, 0, sizeof(state));

	GPIO_StructInit(&gpio);

	// Pins PA0, PA1, PA9 are input buttons
	gpio.GPIO_Mode				= GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed				= GPIO_Speed_50MHz;
	gpio.GPIO_Pin				= BUTTON_PINS;
	GPIO_Init(BUTTON_PORT, &gpio);

	RegisterTimeoutCallback(&PollButtonState, TICK_PRESCALER, CallbackRepeat);

	printf("Buttons initialized\n");
}

void RegisterButtonCallback(uint16_t button, ButtonEventType eventType, button_callback cbk)
{
	for(uint8_t i=0; i<BUTTON_MAX_CBACK; i++)
	{
		if(state[i] == 0)
		{
			state[i] 	= (eventType << 24) | button;
			callback[i] &= CNT_MASK;
			callback[i]	|= (uint32_t)cbk | (eventType << EVENT_SHIFT);
			break;
		}
	}
}

void DeregisterButtonCallbacks()
{
	memset(state, 0, sizeof(state));
}
