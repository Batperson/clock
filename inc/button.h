/*
 * button.h
 *
 *  Created on: 28/10/2019
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#define		SHORT_PRESS_MS		30
#define 	LONG_PRESS_MS		600

#define 	BUTTON_PORT			GPIOA
#define		BUTTON_PINS			GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_9

typedef enum
{
	Short,
	Long
} PressType;

void InitButton();
void PollButtonState();
void OnButtonDown(uint16_t button);
void OnButtonPress(uint16_t button, PressType press);

#endif /* BUTTON_H_ */
