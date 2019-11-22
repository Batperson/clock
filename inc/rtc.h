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

void InitClock();
void SetTime(struct tm* ptm);
void GetTime(struct tm* ptm);
void GetAlarmTime(struct tm* ptm);
enum AlarmFlags GetAlarmFlags();
void SetAlarmTimeAndFlags(struct tm* ptm, enum AlarmFlags flags);
void SnoozeAlarm(uint8_t minutes);

void RTC_OnSecond();
void RTC_OnAlarm();

#endif /* RTC_H_ */
