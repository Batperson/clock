/*
 * button.h
 *
 *  Created on: 28/10/2019
 */

#ifndef BUTTON_H_
#define BUTTON_H_

// Polling interval is divided by the prescaler. So if polling every millisec
// and the prescaler value is 10 then 1 tick = 10 millisecs.
#define		TICK_PRESCALER		10
#define		SHORT_PRESS_TICKS	3
#define 	LONG_PRESS_TICKS	60

#define 	BUTTON_PORT			GPIOA
#define		BUTTON_PINS			GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_9

#define		BUTTON_PIN_MASK		0x0000FFFF
#define		BUTTON_FLAG_MASK	0xFFFF0000
#define		BUTTON_LPRESS		0x80000000

#define		BUTTON_MAX_CBACK	4	// Adjust as necessary

typedef void (*button_callback)();

typedef enum {
	ButtonNone			= 0x00,
	ButtonDown			= 0x01,
	ButtonPress			= 0x02,
	ButtonShort			= 0x04,
	ButtonLong			= 0x08,

	ButtonAny			= ButtonDown | ButtonPress | ButtonShort | ButtonLong,

	ButtonShortDown		= ButtonShort | ButtonDown,
	ButtonShortPress	= ButtonShort | ButtonPress,
	ButtonLongDown		= ButtonLong | ButtonDown,
	ButtonLongPress		= ButtonLong | ButtonPress,
} ButtonEventType;

void InitButton();
void RegisterButtonCallback(uint32_t button, ButtonEventType eventType, button_callback callback);
void DeregisterButtonCallbacks();
void OnButtonDown(uint32_t button);
void OnButtonPress(uint32_t button);

#endif /* BUTTON_H_ */
