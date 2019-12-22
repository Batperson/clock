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
	About
} ClockState;

typedef enum {
	Hour,
	Minute,
	Second,
	Year,
	Month,
	Day
} ClockSetField;

typedef enum {
	None		= 0x00,
	Quick		= 0x01,
	Slow		= 0x02,
} FlashType;

typedef enum {
	AlarmStateNone		= 0x00,
	AlarmSnoozed		= 0x01,
} AlarmState;

typedef enum {
	ModeNone			= 0x00,
	ModeAlarmEnabled	= 0x01,
	ModeAlarmLock		= 0x02,
	ModeAlarmSnooze		= 0x04,
	Mode24HourDisplay	= 0x10
} ClockMode;

typedef struct {
	time_t 			time;
	PSong 			specialSong;
	const char** 	texts;
} SpecialDay, *PSpecialDay;

extern PSpecialDay		specialDay;
extern int16_t			specialDayTextIndex;
extern uint16_t			specialDayYears;
extern char*			specialDayText;
extern ClockState 		clockState;
extern ClockSetField 	clockSetField;
extern AlarmState		alarmState;
extern ClockMode		mode;
extern struct tm 		clockValues;
extern struct tm 		clockSetValues;
extern uint8_t			alarmLock[4];
extern uint8_t			alarmLockIndex;

void SetAlarmRing(uint32_t index);
void SetModeFlags(ClockMode mode);
void ClearModeFlags(ClockMode mode);
void ChangeState(ClockState state);

#endif /* CLOCK_H_ */
