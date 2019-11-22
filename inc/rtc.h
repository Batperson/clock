/*
 * rtc.h
 *
 *  Created on: 21/09/2019
 */

#ifndef RTC_H_
#define RTC_H_

enum AlarmFlags {
	RecurWeekend	= 0x8000,
	RecurWeekday 	= 0x4000,

	AlarmFlagsMask	= 0xF800
};

typedef struct {
	uint8_t 			hour;
	uint8_t				minute;
	enum AlarmFlags		flags;
} Alarm, *PAlarm;

void InitClock();
void SetTime(struct tm* ptm);
void GetTime(struct tm* ptm);
void SetAlarm(PAlarm alm);
void GetAlarm(PAlarm alm);
void SnoozeAlarm(uint8_t minutes);

void RTC_OnSecond();
void RTC_OnAlarm();

#endif /* RTC_H_ */
