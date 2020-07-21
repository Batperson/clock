/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include "stm32f10x.h"
#include "rtc.h"
#include "macros.h"
#include "system.h"
#include "graphics.h"
#include "fonts.h"
#include "sound.h"
#include "song.h"
#include "songs.h"
#include "button.h"
#include "st7735.h"
#include "clock.h"
#include "menu.h"
#include "render.h"

#define BTN_SELECT					GPIO_Pin_9
#define BTN_UP						GPIO_Pin_0
#define BTN_DOWN					GPIO_Pin_1

#define BREG_ALARM_RING				BKP_DR3
#define BREG_CLOCK_MODE				BKP_DR4
#define BREG_SNOOZE_MINS			BKP_DR5
#define BREG_BRIGHTNESS_HOURS		BKP_DR6
#define BREG_BRIGHTNESS_LEVELS		BKP_DR7

const char* birthdayChristopherTexts[] 	= { "Happy birthday Christopher!", "You are now %d years old!", "Hope you have a really nice day today.", NULL };
const char* birthdayRosieTexts[]		= { "Today is Rosie's birthday!", "She is %d today!", "Be really nice to her on her special day!", NULL };
const char* birthdayXiaTexts[]			= { "It's Mum's birthday!", "She is %d today.", "Do something nice for her!", NULL };
const char* birthdayPeterTexts[]		= { "It's Dad's birthday!", "He is %d today.", "Give him a handshake!", NULL };
const char* christmasTexts[]			= { "Around this day in Palestine", "Speaking very approximately", "A child was born", "And the world changed.", "Merry Christmas!", NULL };
const char* newYearTexts[]				= { "HAPPY NEW YEAR!", "MAKE IT A GOOD ONE!", NULL };
const char* waitangiDayTexts[]			= { "Today is Waitangi Day!", "Celebrate your freedoms", "And appreciate your country.", "Remember the past", "And look to the future.", NULL };
const char* anzacDayTexts[]				= { "They shall not grow old", "As we that are left grow old", "Age shall not weary them", "Nor the years condemn.", "But at the going down of the sun", "And in the morning", "We will remember them.", NULL };

const SpecialDay specialDays[] = {
	{ 1093392000, 	&birthday, 		birthdayChristopherTexts },
	{ 1398988800, 	NULL, 			birthdayRosieTexts },
	{ 237427200, 	NULL, 			birthdayXiaTexts },
	{ 184377600, 	NULL, 			birthdayPeterTexts },
	{ 1010793600, 	NULL, 			christmasTexts },
	{ 946684800, 	NULL, 			newYearTexts },
	{ 959904000, 	&anthem,		waitangiDayTexts },
	{ 1010102400, 	NULL,			anzacDayTexts }
};

const PSong alarmRings[] = {
	&reveille,
	&arpeggiator,
	&bbshark,
	&starwars,
	&unknown,
};

MenuItem mainMenu[];

// Max 20 chars per menu item text
MenuItem alarmMenu[] = {
	{ "REVEILLE", 					SetAlarmRing, 		0  },
	{ "ARPEGGIATOR", 				SetAlarmRing, 		1 },
	{ "BABY SHARK", 				SetAlarmRing, 		2 },
	{ "STAR WARS", 					SetAlarmRing, 		3 },
	{ "INTO THE UNKNOWN",			SetAlarmRing, 		4 },

	{ "RANDOM SONG", 				SetAlarmRing,		0xFF },
	{ "BACK", 						SetCurrentMenu, 	(uint32_t)mainMenu  },
	{ NULL, NULL, 0 }
};

MenuItem mainMenu[] = {
	{ "SET TIME", 					SetField, 			ClockHour },
	{ "SET DATE", 					SetField, 			Year },
	{ "SET ALARM TIME", 			SetField, 			AlarmHour  },
	{ "SELECT ALARM RING", 			SetCurrentMenu,		(uint32_t)alarmMenu },
	{ "ALARM ACTIVE", 				SetModeFlags, 		ModeAlarm },
	{ "ALARM SNOOZE",				SetModeFlags,		ModeAlarmSnooze },
	{ "ALARM LOCK",					SetModeFlags, 		ModeAlarmLock },
	{ "24HOUR MODE",				SetModeFlags,		Mode24HourDisplay },
	{ "SET SNOOZE MINUTES",			SetField,			SnoozeMinutes },
	{ "DIM DISPLAY",				SetModeFlags,		ModeNightDimDisplay },
	{ "SET DIM TIME",				SetField,			NighttimeHoursStart },
	{ "SET BRIGHTNESS",				SetField,			DaytimeBrightness },
	{ "CALIBRATE CLOCK",			SetField,			RtcTrim },
	{ "ABOUT THIS CLOCK", 			ChangeState, 		About },
	{ "EXIT", 						ChangeState, 		Normal },
	{ NULL, NULL, 0 }
};

const uint16_t alarmKeys[3] = { BTN_SELECT, BTN_UP, BTN_DOWN };

ClockState 		clockState 			= BootStrap;
ClockSetField 	clockSetField 		= ClockHour;
ClockMode		mode				= ModeAlarmLock | ModeAlarmSnooze | ModeNightDimDisplay;
AlarmState		alarmState			= AlarmStateNone;
PSpecialDay		specialDay			= NULL;
SpecialDayState	specialDayState		= SpecialDayHide;
int16_t			specialDayTextIndex = -1;
uint16_t		specialDayYears		= 0;
uint8_t			snoozeMinutes		= 5;
struct tm 		clockValues;
struct tm		clockSetValues;
BrtStruct		brightnessSettings	= { { 20, 7, 20, 40 } };
uint8_t			alarmLock[4]		= { 0, 0, 0, 0 };
uint8_t			alarmLockIndex		= 0;
uint8_t			alarmRingIndex		= 0;

// 	Pins
//  A12				* USB DP
//  A11				* USB DM
//	A9				Button (Select)
//	A8				Audio
//	A7				Screen (MOSI)
//	A6				Screen (Data/Command)
//	A5				Screen (SCK)
//	A4				Screen (Slave select)
//	A3				Screen (Reset)
// 	A2				Amplifier Shutdown
//	A1				Button (Down)
//	A0				Button (Up)
//  B6				Backlight PWM

void UpdateModeUIAndBehaviour()
{
	if(mode & ModeAlarm) SetAlarmFlags(RecurNone); else SetAlarmFlags(RecurWeekend | RecurWeekday);

	mainMenu[4].flags		= (mode & ModeAlarm) ? MenuSelected : MenuNone;
	mainMenu[4].proc		= (mode & ModeAlarm) ? ClearModeFlags : SetModeFlags;
	mainMenu[5].flags		= (mode & ModeAlarmSnooze) ? MenuSelected : MenuNone;
	mainMenu[5].proc		= (mode & ModeAlarmSnooze) ? ClearModeFlags : SetModeFlags;
	mainMenu[6].flags		= (mode & ModeAlarmLock) ? MenuSelected : MenuNone;
	mainMenu[6].proc		= (mode & ModeAlarmLock) ? ClearModeFlags : SetModeFlags;
	mainMenu[7].flags		= (mode & Mode24HourDisplay) ? MenuSelected : MenuNone;
	mainMenu[7].proc		= (mode & Mode24HourDisplay) ? ClearModeFlags : SetModeFlags;
	mainMenu[9].flags		= (mode & ModeNightDimDisplay) ? MenuSelected : MenuNone;
	mainMenu[9].proc		= (mode & ModeNightDimDisplay) ? ClearModeFlags : SetModeFlags;

	for(int i=0; i < sizeof(alarmMenu) / sizeof(alarmMenu[0]); i++)
		alarmMenu[i].flags = (alarmMenu[i].proc == SetAlarmRing && alarmMenu[i].arg == alarmRingIndex) ? MenuSelected : MenuNone;

	TriggerRender();
}

void OnInitBackupDomain()
{
	// Initialize stored settings. This will only be called when the clock is started for the first time,
	// or if the backup battery goes flat and is replaced.

	BKP_WriteBackupRegister(BREG_CLOCK_MODE, mode);
	BKP_WriteBackupRegister(BREG_ALARM_RING, alarmRingIndex);
	BKP_WriteBackupRegister(BREG_SNOOZE_MINS, snoozeMinutes);
	BKP_WriteBackupRegister(BREG_BRIGHTNESS_HOURS, brightnessSettings.rg.hours);
	BKP_WriteBackupRegister(BREG_BRIGHTNESS_LEVELS, brightnessSettings.rg.levels);
}

void LoadConfiguration()
{
	mode							= BKP_ReadBackupRegister(BREG_CLOCK_MODE);
	alarmRingIndex					= BKP_ReadBackupRegister(BREG_ALARM_RING);
	snoozeMinutes					= BKP_ReadBackupRegister(BREG_SNOOZE_MINS);
	brightnessSettings.rg.hours		= BKP_ReadBackupRegister(BREG_BRIGHTNESS_HOURS);
	brightnessSettings.rg.levels	= BKP_ReadBackupRegister(BREG_BRIGHTNESS_LEVELS);

	UpdateModeUIAndBehaviour();

	// Protection against out of bounds error in case this value somehow gets set incorrectly
	if(alarmRingIndex >= sizeof(alarmRings) / sizeof(alarmRings[0]) && alarmRingIndex != 0xFF)
		alarmRingIndex = 0;

	printf("Config read from backup domain\n");
}

int main(void)
{
 	printf("STM32F103 RTC Alarm Clock v0.01 by Weka Workshop\n");

	// Use all priority bits for pre-emption priority. We want the DMA interrupt for waveform generation
	// to be able to pre-empt everything else.
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	InitClock();
	srand(RTC_GetCounter());
	InitSystem();
	InitDisplay();
	InitSound();
	InitSong();
	InitButton();
	InitRender();

	ClearScreen();
	SetCurrentMenu(mainMenu);
	LoadConfiguration();
	AudioOn();
	ChangeState(Normal);

	while(1)
	{
		// Stop the CPU clock until there is something to do
		// Maybe this will save some milliamps?
		__WFI();
	}
}

void SetModeFlags(ClockMode setMode)
{
	mode |= setMode;

	BKP_WriteBackupRegister(BREG_CLOCK_MODE, mode);
	UpdateModeUIAndBehaviour();
}

void ClearModeFlags(ClockMode clearMode)
{
	mode &= ~clearMode;

	BKP_WriteBackupRegister(BREG_CLOCK_MODE, mode);
	UpdateModeUIAndBehaviour();
}

void AboutHandler(uint16_t btn, ButtonEventType et)
{
	ChangeState(Normal);
}

void ShowMenuHandler(uint16_t btn, ButtonEventType et)
{
	ChangeState(Menu);
}

void MenuHandler(uint16_t btn, ButtonEventType et)
{
	switch(btn)
	{
	case BTN_SELECT:
		MenuSelect();
		break;
	case BTN_UP:
		MenuUp();
		break;
	case BTN_DOWN:
		MenuDown();
		break;
	}
}

static uint16_t lngpress 	= 0;

void MenuLongPressActive()
{
	Beep(88000, 30, 90);
	MenuHandler(lngpress, ButtonShortPress);
}

void MenuLongPressHandler(uint16_t btn, ButtonEventType et)
{
	switch(et)
	{
	case ButtonLongDown:
		lngpress = btn;
		RegisterTimeoutCallback(MenuLongPressActive, 70, CallbackRepeat);
		break;
	case ButtonLongPress:
		DeregisterCallback(MenuLongPressActive);
		break;
	default:
		break;
	}
}

void OnMenuHighlight(PMenuItem item)
{
	EndSong();
}

void OnMenuTimeout(PMenuItem item)
{
	if(item->proc == SetAlarmRing)
	{
		PSong song = alarmRings[item->arg];

		SelectSong(song);
		PlaySong(PlayLoop);
	}
}

int GetMaxDay(uint32_t year, uint8_t month)
{
	/*
	 * Thirty days hath September April June and November
	 * All the rest have 31 except February alone which has 28 days clear
	 * And 29 each leap year.
	 * And there is a leap year if the year % 4 == 0, unless year % 100 == 0 and year % 400 != 0
	 * The only year within a unix timestamp where the last 2 criteria can apply is 2000
	 * So we don't need to worry about Gregorian leap years.
	 */
	switch(month)
	{
	case 1:
		if(year % 4 == 0)
			return 29;
		return 28;
	case 3:
	case 8:
	case 10:
		return 30;
	default:
		return 31;
	}
}

void FieldSetDown()
{
	switch(clockSetField)
	{
	case ClockHour:
	case AlarmHour:
	case NighttimeHoursStart:
	case NighttimeHoursEnd:
		if(clockSetValues.tm_hour-- == 0) clockSetValues.tm_hour = 23;
		break;
	case ClockMinute:
	case AlarmMinute:
		if(clockSetValues.tm_min-- == 0) clockSetValues.tm_min = 59;
		break;
	case ClockSecond:
		clockSetValues.tm_sec = 0;
		break;
	case Year:
		if(clockSetValues.tm_year-- <= 02) clockSetValues.tm_year = 02;	// Around the smallest possible timestamp year
		break;
	case Month:
		if(clockSetValues.tm_mon-- == 0) clockSetValues.tm_mon = 11;
		break;
	case Day:
		if(clockSetValues.tm_mday-- == 1) clockSetValues.tm_mday = GetMaxDay(clockSetValues.tm_year, clockSetValues.tm_mon);
		break;
	case SnoozeMinutes:
		if(clockSetValues.tm_yday-- == MIN_SNOOZE_MINUTES) clockSetValues.tm_yday = MAX_SNOOZE_MINUTES;
		break;
	case RtcTrim:
		if(clockSetValues.tm_yday-- == 0) clockSetValues.tm_yday = MAX_RTC_TRIM;
	case DaytimeBrightness:
		if(brightnessSettings.bs.daytimeBrightness-- == MIN_BRIGHTNESS) brightnessSettings.bs.daytimeBrightness = MIN_BRIGHTNESS;
		SetBacklightLevel(brightnessSettings.bs.daytimeBrightness);
		break;
	case NighttimeBrightness:
		if(brightnessSettings.bs.nighttimeBrightness-- == MIN_BRIGHTNESS) brightnessSettings.bs.nighttimeBrightness = MIN_BRIGHTNESS;
		SetBacklightLevel(brightnessSettings.bs.nighttimeBrightness);
		break;
	default:
		break;
	}

	TriggerRender();
}

void FieldSetUp()
{
	switch(clockSetField)
	{
	case ClockHour:
	case AlarmHour:
	case NighttimeHoursStart:
	case NighttimeHoursEnd:
		if(clockSetValues.tm_hour++ == 23) clockSetValues.tm_hour = 0;
		break;
	case ClockMinute:
	case AlarmMinute:
		if(clockSetValues.tm_min++ == 59) clockSetValues.tm_min = 0;
		break;
	case ClockSecond:
		clockSetValues.tm_sec = 0;
		break;
	case Year:
		if(clockSetValues.tm_year++ >= 138) clockSetValues.tm_year = 138;	// Max possible timestamp year
		break;
	case Month:
		if(clockSetValues.tm_mon++ == 11) clockSetValues.tm_mon = 0;
		break;
	case Day:
		if(clockSetValues.tm_mday++ == GetMaxDay(clockSetValues.tm_year, clockSetValues.tm_mon)) clockSetValues.tm_mday = 1;
		break;
	case SnoozeMinutes:
		if(clockSetValues.tm_yday++ == MAX_SNOOZE_MINUTES) clockSetValues.tm_yday = MIN_SNOOZE_MINUTES;
		break;
	case RtcTrim:
		if(clockSetValues.tm_yday++ == MAX_RTC_TRIM) clockSetValues.tm_yday = 0;
		break;
	case DaytimeBrightness:
		if(brightnessSettings.bs.daytimeBrightness++ == MAX_BRIGHTNESS) brightnessSettings.bs.daytimeBrightness = MAX_BRIGHTNESS;
		SetBacklightLevel(brightnessSettings.bs.daytimeBrightness);
		break;
	case NighttimeBrightness:
		if(brightnessSettings.bs.nighttimeBrightness++ == MAX_BRIGHTNESS) brightnessSettings.bs.nighttimeBrightness = MAX_BRIGHTNESS;
		SetBacklightLevel(brightnessSettings.bs.nighttimeBrightness);
		break;
	default:
		break;
	}

	TriggerRender();
}

void FieldMoveNext()
{
	switch(clockSetField)
	{
	case ClockHour:
	case ClockMinute:
	case Year:
	case Month:
	case AlarmHour:
		clockSetField++;
		break;
	case ClockSecond:
		SetTime(&clockSetValues);
		memcpy(&clockValues, &clockSetValues, sizeof(struct tm));

		ChangeState(Normal);
		break;
	case Day:
		SetTime(&clockSetValues);
		memcpy(&clockValues, &clockSetValues, sizeof(struct tm));

		ChangeState(Normal);
		break;
	case AlarmMinute:
		SetAlarmTime(&clockSetValues);
		ChangeState(Normal);
		break;
	case SnoozeMinutes:
		snoozeMinutes = clockSetValues.tm_yday;
		BKP_WriteBackupRegister(BREG_SNOOZE_MINS, snoozeMinutes);
		ChangeState(Normal);
		break;
	case RtcTrim:
		SetRtcCalibrationValue(clockSetValues.tm_yday);
		ChangeState(Normal);
		break;
	case NighttimeHoursStart:
		brightnessSettings.bs.nightHoursStart = clockSetValues.tm_hour;
		clockSetValues.tm_hour = brightnessSettings.bs.nightHoursEnd;
		clockSetField++;
		break;
	case NighttimeHoursEnd:
		brightnessSettings.bs.nightHoursEnd = clockSetValues.tm_hour;
		BKP_WriteBackupRegister(BREG_BRIGHTNESS_HOURS, brightnessSettings.rg.hours);
		ChangeState(Normal);
		break;
	case DaytimeBrightness:
		clockSetField++;
		SetBacklightLevel(brightnessSettings.bs.nighttimeBrightness);
		break;
	case NighttimeBrightness:
		BKP_WriteBackupRegister(BREG_BRIGHTNESS_LEVELS, brightnessSettings.rg.levels);
		ChangeState(Normal);
		break;
	default:
		ChangeState(Normal);
		break;
	}

	ClearScreen();
	TriggerRender();
}

void FieldPressHandler(uint16_t btn, ButtonEventType et)
{
	switch(btn)
	{
	case BTN_SELECT:
		FieldMoveNext();
		break;
	case BTN_UP:
		FieldSetUp();
		break;
	case BTN_DOWN:
		FieldSetDown();
		break;
	}
}
void FieldLongPressActive()
{
	Beep(88000, 30, 90);
	FieldPressHandler(lngpress, ButtonShortPress);
}

void FieldLongPressHandler(uint16_t btn, ButtonEventType et)
{
	switch(et)
	{
	case ButtonLongDown:
		lngpress = btn;
		RegisterTimeoutCallback(FieldLongPressActive, 70, CallbackRepeat);
		break;
	case ButtonLongPress:
		DeregisterCallback(FieldLongPressActive);
		break;
	default:
		break;
	}
}

void SetAlarmLock()
{
	for(int i=0; i<(sizeof(alarmLock) / sizeof(alarmLock[0])); i++)
		alarmLock[i]	= rand() % (sizeof(alarmKeys) / sizeof(alarmKeys[0]));

	alarmLockIndex	= 0;
}

void AlarmButtonHandler(uint16_t btn, ButtonEventType et)
{
	uint8_t getAlarmKeyIndex(uint16_t btn) {
		for(uint8_t i=0; i<3; i++)
			if(btn == alarmKeys[i])
				return i;

		return 0;
	}

	void stopAlarm() {
		alarmState 		= AlarmStateNone;
		alarmLockIndex 	= 0;
		ChangeState(Normal);
	}

	void resetLock() {
		alarmLockIndex = 0;
		TriggerRender();
	}

	if(et == ButtonLongDown)
	{
		if((mode & ModeAlarmSnooze) && !(alarmState & AlarmStateSnoozed))
		{
			alarmState |= AlarmStateSnoozed;
			SnoozeAlarm(snoozeMinutes);
		}
	}
	else if(et == ButtonShortPress)
	{
		if(mode & ModeAlarmLock)
		{
			if(getAlarmKeyIndex(btn) == alarmLock[alarmLockIndex])
			{
				if(alarmLockIndex++ == 3)
					stopAlarm();

				TriggerRender();
			}
			else
			{
				resetLock();
			}
		}
		else
		{
			stopAlarm();
		}
	}
}

void SetField(ClockSetField field)
{
	clockSetField = field;

	switch(clockSetField)
	{
	case AlarmHour:
	case AlarmMinute:
		GetAlarmTime(&clockSetValues);
		break;
	case RtcTrim:
		clockSetValues.tm_yday	= GetRtcCalibrationValue();
		break;
	case SnoozeMinutes:
		clockSetValues.tm_yday = snoozeMinutes;
		break;
	case NighttimeHoursStart:
		clockSetValues.tm_hour = brightnessSettings.bs.nightHoursStart;
		break;
	case NighttimeHoursEnd:
		clockSetValues.tm_hour = brightnessSettings.bs.nightHoursEnd;
		break;
	default:
		GetTime(&clockSetValues);
		break;
	}

	ChangeState(FieldSet);
}

uint8_t IsNightTime()
{
	return (clockValues.tm_hour >= brightnessSettings.bs.nightHoursStart || clockValues.tm_hour < brightnessSettings.bs.nightHoursEnd) ? 1 : 0;
}

void ChangeState(ClockState state)
{
	if(clockState != state)
	{
		clockState = state;

		DeregisterCallback(TriggerRender);
		DeregisterButtonCallbacks();
		EndSong();

		switch(clockState)
		{
		case About:
			SelectSong(&gravity);
			PlaySong(PlayLoop);
			RegisterButtonCallback(BTN_SELECT | BTN_UP | BTN_DOWN, ButtonAny, AboutHandler);
			RegisterTimeoutCallback(TriggerRender, 100, CallbackRepeat);
			SetBacklightLevel(MAX_BRIGHTNESS);
			break;

		case Menu:
			RegisterButtonCallback(BTN_SELECT | BTN_UP | BTN_DOWN, ButtonShortPress, MenuHandler);
			RegisterButtonCallback(BTN_UP | BTN_DOWN, ButtonLongDown | ButtonLongPress, MenuLongPressHandler);
			SetBacklightLevel(MAX_BRIGHTNESS);
			break;

		case AlarmRing:
			RegisterButtonCallback(BTN_SELECT | BTN_UP | BTN_DOWN, ButtonLongDown | ButtonShortPress, AlarmButtonHandler);
			RegisterTimeoutCallback(TriggerRender, 100, CallbackRepeat);
			SetBacklightLevel(MAX_BRIGHTNESS);
			SetAlarmLock();
			break;

		case FieldSet:
			RegisterButtonCallback(BTN_UP | BTN_DOWN | BTN_SELECT, ButtonShortPress, FieldPressHandler);
			RegisterButtonCallback(BTN_UP | BTN_DOWN, ButtonLongDown | ButtonLongPress, FieldLongPressHandler);
			switch(clockSetField)
			{
			case DaytimeBrightness:
				SetBacklightLevel(brightnessSettings.bs.daytimeBrightness);
				break;
			case NighttimeBrightness:
				SetBacklightLevel(brightnessSettings.bs.nighttimeBrightness);
				break;
			default:
				SetBacklightLevel(MAX_BRIGHTNESS);
				break;
			}

			break;

		case Normal:
		default:
			SetBacklightLevel(IsNightTime() ? brightnessSettings.bs.nighttimeBrightness : brightnessSettings.bs.daytimeBrightness);
			RegisterButtonCallback(BTN_SELECT, ButtonLongDown, ShowMenuHandler);
			break;
		}

		ClearScreen();
		TriggerRender();
	}
}

void SetAlarmRing(uint32_t index)
{
	if(index < (sizeof(alarmRings) / sizeof(PSong)) && index != 0xFF)
	{
		alarmRingIndex = index;

		BKP_WriteBackupRegister(BREG_ALARM_RING, index);

		TriggerRender();

		for(int i=0; i < sizeof(alarmMenu) / sizeof(alarmMenu[0]); i++)
			alarmMenu[i].flags = (alarmMenu[i].proc == SetAlarmRing && alarmMenu[i].arg == alarmRingIndex) ? MenuSelected : MenuNone;
	}
}

void OnRtcSecond()
{
	GetTime(&clockValues);

	if(++clockSetValues.tm_sec >= 60)
		clockSetValues.tm_sec = 0;

	switch(clockState)
	{
	case Menu:
	case About:
	case AlarmRing:
		break;
	case FieldSet:
		if(clockSetField == ClockSecond) TriggerRender();
		break;
	default:
		TriggerRender();
		break;
	}

	static int yday = -1;
	if(clockValues.tm_yday != yday)
	{
		SpecialDayState tmpState 	= SpecialDayHide;
		yday 						= clockValues.tm_yday;

		// Clear the text banner area
		DrawRect(0, 94, 162, 12, DrawNormal);

		for(int i=0; i<sizeof(specialDays) / sizeof(SpecialDay); i++)
		{
			struct tm* ptm 				= localtime(&specialDays[i].time);

			if(ptm->tm_mday == clockValues.tm_mday && ptm->tm_mon == clockValues.tm_mon)
			{
				specialDay 			= (PSpecialDay)&specialDays[i];
				specialDayTextIndex	= -1;
				specialDayYears 	= clockValues.tm_year - ptm->tm_year;
				tmpState			= SpecialDayShow;
				break;
			}
		}

		specialDayState = tmpState;
	}

	static int hr = -1;
	if(clockValues.tm_hour != hr)
	{
		hr				= clockValues.tm_hour;

		if(clockState == Normal)
			SetBacklightLevel(
				IsNightTime() ?
					brightnessSettings.bs.nighttimeBrightness :
					brightnessSettings.bs.daytimeBrightness);
	}
}

void OnRtcAlarm()
{
	// Ignore an alarm if the user is busy with the menu
	if(clockState != Menu)
	{
		if(clockState != AlarmRing || alarmState & AlarmStateSnoozed)
		{
			alarmState = AlarmStateNone;

			SelectSong((specialDay != NULL && specialDay->specialSong != NULL) ? specialDay->specialSong : alarmRings[(alarmRingIndex == 0xFF) ? rand() % (sizeof(alarmRings) / sizeof(alarmRings[0])) : alarmRingIndex]);
			PlaySong(PlayLoop);
		}

		ChangeState(AlarmRing);
	}
}

void OnButtonEvent(uint32_t btn, ButtonEventType eventType)
{
	printf("btn: %d\t%d\n", (int)btn, eventType);

	switch(eventType)
	{
	case ButtonShortPress:
		Beep(88000, 60, 90);
		break;

	case ButtonLongDown:
		Beep(88000, 100, 90);
		break;

	default:
		break;
	}
}


