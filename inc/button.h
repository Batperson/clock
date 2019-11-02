/*
 * button.h
 *
 *  Created on: 28/10/2019
 */

#ifndef BUTTON_H_
#define BUTTON_H_

// Polling interval is divided by the prescaler. So if polling every millisec
// and the prescaler value is 4 then 1 tick = 4 millisecs.
#define		TICK_PRESCALER		4
#define		SHORT_PRESS_TICKS	8
#define 	LONG_PRESS_TICKS	150

#define 	BUTTON_PORT			GPIOA
#define		BUTTON_PINS			GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_9

typedef enum
{
	Short,
	Long
} PressType;

void InitButton();
void PollButtonState();
void BTN_OnDown(uint16_t button, PressType press);
void BTN_OnPress(uint16_t button, PressType press);

#endif /* BUTTON_H_ */
