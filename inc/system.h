/*
 * system.h
 *
 *  Created on: 22/09/2019
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

void InitSystem();

void sleep(uint32_t ms);

uint32_t millis();

#define MAX_CALLBACKS	4

typedef void (*callback_ptr)();
void RegisterSysTickCallback(callback_ptr ptr);
void RegisterTimeoutCallback(callback_ptr ptr, uint32_t millis);
void DeregisterCallback(callback_ptr);

#endif /* SYSTEM_H_ */
