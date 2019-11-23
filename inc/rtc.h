/*
 * rtc.h
 *
 *  Created on: 21/09/2019
 */

#ifndef RTC_H_
#define RTC_H_

typedef enum {
	RecurWeekend	= 0x8000,
	RecurWeekday 	= 0x4000,

	AlarmFlagsMask	= 0xF800
} AlarmFlags;

void InitClock();
void SetTime(struct tm* ptm);
void GetTime(struct tm* ptm);
void GetAlarmTime(struct tm* ptm);
AlarmFlags GetAlarmFlags();
void SetAlarmTimeAndFlags(struct tm* ptm, AlarmFlags flags);
void SnoozeAlarm(uint8_t minutes);

void RTC_OnSecond();
void RTC_OnAlarm();

#endif /* RTC_H_ */
