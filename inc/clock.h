/*
 * clock.h
 *
 *  Created on: 7/11/2019
 */

#ifndef CLOCK_H_
#define CLOCK_H_

typedef enum {
	Normal,
	Menu,
	ClockSet,
	AlarmSet,
	DateSet,
	AlarmRing,
	AlarmSnooze,
	About
} ClockState;

typedef enum {
	Hour,
	Minute,
	Second,

	Year	= Hour,
	Month	= Minute,
	Day		= Second
} ClockSetField;

typedef enum {
	None		= 0x00,
	Quick		= 0x01,
	Slow		= 0x02,
} FlashType;

typedef enum {
	AlarmDisabled,
	AlarmEnabled
} AlarmState;

typedef struct {
	time_t 			time;
	PSong 			specialSong;
	const char** 	texts;
} SpecialDay, *PSpecialDay;

extern PSpecialDay		specialDay;
extern uint16_t			specialDayYears;
extern char*			specialDayText;
extern ClockState 		clockState;
extern ClockSetField 	clockSetField;
extern AlarmState		alarmState;
extern struct tm 		clockValues;
extern struct tm 		clockSetValues;

void SetAlarmRing(PSong song);
void SetAlarmState(AlarmState state);
void ChangeState(ClockState state);

#endif /* CLOCK_H_ */
