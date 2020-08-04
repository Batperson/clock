/*
 * clock.h
 *
 *  Created on: 7/11/2019
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#define MIN_SNOOZE_MINUTES	5
#define MAX_SNOOZE_MINUTES	15
#define MAX_BRIGHTNESS		255
#define MIN_BRIGHTNESS		2

typedef enum {
	Normal,
	Menu,
	FieldSet,
	AlarmRing,
	About,
	TextDemo,

	BootStrap	= 0xFF
} ClockState;

typedef enum {
	AlarmHour,
	AlarmMinute,
	ClockHour,
	ClockMinute,
	ClockSecond,
	Year,
	Month,
	Day,
	SnoozeMinutes,
	RtcTrim,
	DaytimeBrightness,
	NighttimeBrightness,
	NighttimeHoursStart,
	NighttimeHoursEnd
} ClockSetField;

typedef enum {
	None		= 0x00,
	Quick		= 0x01,
	Slow		= 0x02,
} FlashType;

typedef enum {
	AlarmStateNone		= 0x00,
	AlarmStateSnoozed	= 0x01,
} AlarmState;

typedef enum {
	ModeNone			= 0x00,
	ModeAlarm			= 0x01,
	ModeAlarmLock		= 0x02,
	ModeAlarmSnooze		= 0x04,
	Mode24HourDisplay	= 0x10,
	ModeNightDimDisplay	= 0x20
} ClockMode;

typedef struct {
	time_t 			time;
	PSong 			specialSong;
	const char** 	texts;
} SpecialDay, *PSpecialDay;

typedef enum {
	SpecialDayHide	 	= 0x00,
	SpecialDayShow		= 0x01
} SpecialDayState;

typedef union {
	struct {
		uint8_t			nightHoursStart;
		uint8_t			nightHoursEnd;
		uint8_t			nighttimeBrightness;
		uint8_t			daytimeBrightness;
	} bs;
	struct {
		uint16_t		hours;
		uint16_t		levels;
	} rg;
} BrtStruct;

extern BrtStruct		brightnessSettings;
extern PSpecialDay		specialDay;
extern SpecialDayState	specialDayState;
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
void SetField(ClockSetField field);

#endif /* CLOCK_H_ */
