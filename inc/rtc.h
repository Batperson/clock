/*
 * rtc.h
 *
 *  Created on: 21/09/2019
 */

#ifndef RTC_H_
#define RTC_H_

void InitClock();
void SetTime(struct tm* ptm);
void GetTime(struct tm* ptm);

void RTC_OnSecond();
void RTC_OnAlarm();

#endif /* RTC_H_ */
