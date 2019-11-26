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

uint8_t 	prescaler;
uint32_t	state[BUTTON_MAX_CBACK];
uint16_t	press;

#define EVENT_TYPE_MASK			0xF0000000	// >> 28
#define COUNTER_MASK			0x007E0000	// >> 17
#define FUNC_PTR_MASK			0x0801FFFF
#define PIN_MASK				0x07800000  // >> 23

// LONG_PRESS_TICKS must not exceed this value or the event will never occur.
#define MAX_TICK_COUNT			0x3F		// 63

void WEAKREF OnButtonEvent(uint32_t button, ButtonEventType eventType)
{

}

// To be called from a repeating interrupt such as SysTick, ideally 1KHz.
void PollButtonState()
{
	if(prescaler++ >= TICK_PRESCALER)
	{
		prescaler = 0;

		for(uint8_t i=0; i<BUTTON_MAX_CBACK; i++)
		{
			uint32_t val = state[i];
			if(val != 0)
			{
				uint32_t pin		= 1 << ((val & PIN_MASK) >> 23);
				uint32_t ctr		= (val & COUNTER_MASK) >> 17;
				button_callback cbk	= (button_callback)(val & FUNC_PTR_MASK);
				ButtonEventType et 	= (val & EVENT_TYPE_MASK) >> 28;
				ButtonEventType eta = ButtonNone;

				if(pin & BUTTON_PORT->IDR & ~press)
				{
					if(ctr == SHORT_PRESS_TICKS)
						eta = ButtonShortDown;
					else if(ctr == LONG_PRESS_TICKS)
						eta = ButtonLongDown;

					if(eta == ButtonLongDown)
						press |= pin;
					if(ctr < MAX_TICK_COUNT)
						ctr++;
				}
				else if(!(pin & BUTTON_PORT->IDR))
				{
					if(ctr >= LONG_PRESS_TICKS)
						eta = ButtonLongPress;
					else if(ctr >= SHORT_PRESS_TICKS)
						eta = ButtonShortPress;

					press	&= ~pin;
					ctr 	= 0;
				}

				val			&= ~COUNTER_MASK;
				val			|= ctr << 17;
				state[i]	= val;

				if(eta != ButtonNone)
				{
					OnButtonEvent(pin, eta);

					if((eta & et) == eta)
						cbk();
				}
			}
		}
	}
}

void InitButton()
{
	GPIO_InitTypeDef 			gpio;

	memset(state, 0, sizeof(state));
	prescaler 					= 0;
	press						= 0;

	GPIO_StructInit(&gpio);

	// Pins PA0, PA1, PA9 are input buttons
	gpio.GPIO_Mode				= GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed				= GPIO_Speed_50MHz;
	gpio.GPIO_Pin				= BUTTON_PINS;
	GPIO_Init(BUTTON_PORT, &gpio);

	RegisterSysTickCallback(&PollButtonState);

	printf("Buttons initialized\n");
}

void RegisterButtonCallback(uint32_t button, ButtonEventType eventType, button_callback callback)
{
	uint8_t btn			= 0;
	for(uint32_t b = button; b > 1; b >>= 1)
		btn++;

	uint32_t val		= (uint32_t)callback;
	val					|= eventType << 28;
	val					|= (btn << 23);

	for(uint8_t i=0; i<BUTTON_MAX_CBACK; i++)
	{
		if(state[i] == 0)
		{
			state[i] = val;
			break;
		}
	}
}

void DeregisterButtonCallbacks()
{
	memset(state, 0, sizeof(state));
}
