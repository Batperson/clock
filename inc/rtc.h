/*
 * rtc.h
 *
 *  Created on: 21/09/2019
 */

#ifndef RTC_H_
#define RTC_H_

typedef enum {
	RecurNone		= 0x0000,
	RecurWeekend	= 0x8000,
	RecurWeekday 	= 0x4000,

	AlarmFlagsMask	= 0xF800
} AlarmFlags;

void InitClock();
void SetTime(struct tm* ptm);
void GetTime(struct tm* ptm);
void GetAlarmTime(struct tm* ptm);
AlarmFlags GetAlarmFlags();
void SetAlarmFlags(AlarmFlags flags);
void SetAlarmTimeAndFlags(struct tm* ptm, AlarmFlags flags);
void SetAlarmTime(struct tm* ptm);
void SnoozeAlarm(uint8_t minutes);

void OnRtcSecond();
void OnRtcAlarm();

#endif /* RTC_H_ */
