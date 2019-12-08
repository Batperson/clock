/*
 * system.h
 *
 *  Created on: 22/09/2019
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

void InitSystem();

void sleep(uint32_t ms);

#define MAX_CALLBACKS	6

typedef void (*callback_ptr)();

typedef enum {
	CallbackNone	= 0x00000000,
	CallbackRepeat	= 0x80000000
} CallbackFlags;

// Note millis max value is 31
void RegisterTimeoutCallback(callback_ptr ptr, uint16_t millis, CallbackFlags flags);
void DeregisterCallback(callback_ptr);

#endif /* SYSTEM_H_ */
