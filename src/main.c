/* Includes */
#include <stdio.h>
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
#include "button.h"
#include "st7735.h"
#include "clock.h"
#include "menu.h"
#include "render.h"

#define BTN_SELECT					GPIO_Pin_9
#define BTN_UP						GPIO_Pin_0
#define BTN_DOWN					GPIO_Pin_1

extern Song arpeggiator;
extern Song reveille;

const char* birthdayChristopherTexts[] 	= { "Happy birthday Christopher!", "You are now %d years old!", "Hope you have a really nice day today.", NULL };
const char* birthdayRosieTexts[]		= { "Today is Rosie's birthday!", "She is %d today!", "Be really nice to her on her special day!", NULL };
const char* birthdayXiaTexts[]			= { "It's Mum's birthday!", "She is %d today.", "Do something nice for her!", NULL };
const char* birthdayPeterTexts[]		= { "It's Dad's birthday!", "He is %d today.", "Give him a handshake!", NULL };
const char* christmasTexts[]			= { "Around this day in Palestine", "A child was born", "And the world changed.", "Merry Christmas!", NULL };
const char* newYearTexts[]				= { "Happy New Year!", NULL };
const char* waitangiDayTexts[]			= { "Today is Waitangi Day!", "Celebrate your freedoms", "And appreciate your country.", "Remember the past", "And look to the future.", NULL };
const char* anzacDayTexts[]				= { "They shall not grow old", "As we that are left grow old", "Age shall not weary them", "Nor the years condemn.", "But at the going down of the sun", "And in the morning", "We will remember them.", NULL };

SpecialDay specialDays[] = {
	{ 65535, 	NULL, 			birthdayChristopherTexts },
	{ 65535, 	NULL, 			birthdayRosieTexts },
	{ 65535, 	NULL, 			birthdayXiaTexts },
	{ 65535, 	NULL, 			birthdayPeterTexts },
	{ 65535, 	NULL, 			christmasTexts },
	{ 0, 		NULL, 			newYearTexts },
	{ 65535, 	NULL, 			waitangiDayTexts }
};

MenuItem mainMenu[];

MenuItem alarmMenu[] = {
	{ "SET ALARM ON", 				SetAlarmState, 		AlarmEnabled },
	{ "SET ALARM TIME", 			ChangeState, 		AlarmSet  },
	{ "RING: REVEILLE", 			SetAlarmRing, 		(uint32_t)&reveille  },
	{ "RING: ARPEGGIATOR", 			SetAlarmRing, 		(uint32_t)&arpeggiator },
	{ "BACK", 						SetCurrentMenu, 	(uint32_t)mainMenu  },
	{ NULL, NULL, 0 }
};

MenuItem mainMenu[] = {
	{ "SET TIME", 					ChangeState, 	ClockSet },
	{ "SET DATE", 					ChangeState, 	DateSet },
	{ "SET ALARM", 					SetCurrentMenu,	(uint32_t)alarmMenu },
	{ "ABOUT THIS CLOCK", 			ChangeState, 	About },
	{ "EXIT", 						ChangeState, 	Normal },
	{ NULL, NULL, 0 }
};

PSong			alarmRing			= &reveille;
ClockState 		clockState 			= Normal;
ClockSetField 	clockSetField 		= Hour;
ClockFormat		clockFormat			= Format24Hour;
AlarmState		alarmState			= AlarmDisabled;
PSpecialDay		specialDay			= NULL;
char*			specialDayText		= NULL;
uint16_t		specialDayYears		= 0;
uint8_t			snoozeMinutes		= 5;
struct tm 		clockValues;
struct tm		clockSetValues;

// 	Pins
//	A9				Button (Select)
//	A8				Audio
//	A7				Screen (MOSI)
//	A6				Screen (Data/Command)
//	A5				Screen (SCK)
//	A4				Screen (Slave select)
//	A3				Screen (Reset)
// 	A2				Screen (Backlight)
//	A1				Button (Down)
//	A0				Button (Up)

void LoadConfiguration()
{
	// TODO: Read alarm state etc from backup domain
	printf("Config read from backup domain\n");
}

int main(void)
{
	printf("STM32F103 RTC Alarm Clock v0.01 by Weka Workshop\n");

	// Use all priority bits for pre-emption priority. We want the DMA interrupt for waveform generation
	// to be able to pre-empt everything else.
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	InitClock();
	InitSystem();
	InitDisplay();
	InitSound();
	InitSong();
	InitButton();
	InitRender();

	ClearScreen();
	SetCurrentMenu(mainMenu);
	ChangeState(Normal);
	LoadConfiguration();

	while(1)
	{
		// Stop the CPU clock until there is something to do
		// Maybe this will save some milliamps?
		//__WFI();
	}
}

void SetAlarmState(AlarmState als)
{
	if(als == AlarmEnabled)
	{
		alarmState 			= AlarmEnabled;
		alarmMenu[0].text	= "SET ALARM OFF";
		alarmMenu[0].arg	= AlarmDisabled;
	}
	else
	{
		alarmState 			= AlarmDisabled;
		alarmMenu[0].text	= "SET ALARM ON";
		alarmMenu[0].arg	= AlarmEnabled;
	}
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

void OnMenuHighlight(PMenuItem item)
{
	EndSong();
}

void OnMenuTimeout(PMenuItem item)
{
	if(item->proc == SetAlarmRing)
	{
		SelectSong((PSong)item->arg);
		PlaySong();
	}
}

int GetMaxDay(uint32_t year, uint8_t month)
{
	/*
	 * Thirty days hath September April June and November
	 * All the rest have 31 except February alone which has 28 days clear
	 * And 29 each leap year.
	 * And there is a leap year if the year % 4 == 0, unless year % 100 == 0 and year % 400 != 0
	 * The only year within a unix timestamp where the last 2 criteria can apply is 2000.
	 */
	switch(month)
	{
	case 1:
		if(year % 4 == 0)
			return 29;
		if(year == 2000)
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

void FieldSetUp()
{
	switch(clockSetField)
	{
	case Hour:
		if(clockSetValues.tm_hour-- == 0) clockSetValues.tm_hour = 23;
		break;
	case Minute:
		if(clockSetValues.tm_min-- == 0) clockSetValues.tm_min = 59;
		break;
	case Second:
		clockSetValues.tm_sec = 0;
		break;
	case Year:
		if(clockSetValues.tm_year-- == 1902) clockSetValues.tm_year = 1902;	// Around the smallest possible timestamp year
		break;
	case Month:
		if(clockSetValues.tm_mon-- == 0) clockSetValues.tm_mon = 11;
		break;
	case Day:
		if(clockSetValues.tm_mday-- == 1) clockSetValues.tm_mday = GetMaxDay(clockSetValues.tm_year, clockSetValues.tm_mon);
		break;
	default:
		break;
	}

	TriggerRender();
}

void FieldSetDown()
{
	switch(clockSetField)
	{
	case Hour:
		if(clockSetValues.tm_hour++ == 23) clockSetValues.tm_hour = 0;
		break;
	case Minute:
		if(clockSetValues.tm_min++ == 59) clockSetValues.tm_min = 0;
		break;
	case Second:
		clockSetValues.tm_sec = 0;
		break;
	case Year:
		if(clockSetValues.tm_year++ == 2038) clockSetValues.tm_year = 2038;	// Max possible timestamp year
		break;
	case Month:
		if(clockSetValues.tm_mon++ == 11) clockSetValues.tm_mon = 0;
		break;
	case Day:
		if(clockSetValues.tm_mday++ == GetMaxDay(clockSetValues.tm_year, clockSetValues.tm_mon)) clockSetValues.tm_mday = 1;
		break;
	default:
		break;
	}

	TriggerRender();
}

void FieldMoveNext()
{
	switch(clockState)
	{
	case ClockSet:
		if(clockSetField++ >= Second)
		{
			SetTime(&clockSetValues);
			GetTime(&clockValues);
			ChangeState(Normal);
		}
		break;
	case DateSet:
		if(clockSetField++ >= Day)
		{
			SetTime(&clockSetValues);
			GetTime(&clockValues);
			ChangeState(Normal);
		}
		break;
	case AlarmSet:
		if(clockSetField++ >= Minute)
		{
			SetAlarmTime(&clockSetValues);
			ChangeState(Normal);
		}
		break;
	default:
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

static uint16_t lngpress 	= 0;
static uint8_t stprescaler	= 0;
void FieldLongPressActive()
{
	if(stprescaler++ >= 70)
	{
		Beep(88000, 30, 90);
		FieldPressHandler(lngpress, ButtonShortPress);
		stprescaler = 0;
	}
}

void FieldLongPressHandler(uint16_t btn, ButtonEventType et)
{
	switch(et)
	{
	case ButtonLongDown:
		lngpress = btn;
		RegisterSysTickCallback(FieldLongPressActive);
		break;
	case ButtonLongPress:
		DeregisterCallback(FieldLongPressActive);
		break;
	default:
		break;
	}
}

void ChangeState(ClockState state)
{
	clockState = state;

	DeregisterButtonCallbacks();
	EndSong();

	switch(clockState)
	{
	case About:
		RegisterButtonCallback(BTN_SELECT | BTN_UP | BTN_DOWN, ButtonAny, AboutHandler);
		break;

	case Menu:
		RegisterButtonCallback(BTN_SELECT | BTN_UP | BTN_DOWN, ButtonShortPress, MenuHandler);
		break;

	case AlarmRing:
		SelectSong((specialDay != NULL && specialDay->specialSong != NULL) ? specialDay->specialSong : alarmRing);
		PlaySong();
		//RegisterButtonCallback(BTN_SELECT, ButtonShortPress, SnoozeAlarm);
		//RegisterButtonCallback(BTN_UP, ButtonShortPress, SnoozeAlarm);
		//RegisterButtonCallback(BTN_DOWN, ButtonShortPress, SnoozeAlarm);
		break;

	case ClockSet:
		clockSetField = Hour;
		GetTime(&clockSetValues);
		RegisterButtonCallback(BTN_UP | BTN_DOWN | BTN_SELECT, ButtonShortPress, FieldPressHandler);
		RegisterButtonCallback(BTN_UP | BTN_DOWN, ButtonLongDown | ButtonLongPress, FieldLongPressHandler);
		break;

	case DateSet:
		clockSetField = Year;
		GetTime(&clockSetValues);
		RegisterButtonCallback(BTN_UP | BTN_DOWN | BTN_SELECT, ButtonShortPress, FieldPressHandler);
		RegisterButtonCallback(BTN_UP | BTN_DOWN, ButtonLongDown | ButtonLongPress, FieldLongPressHandler);
		break;

	case AlarmSet:
		clockSetField = Hour;
		GetAlarmTime(&clockSetValues);
		RegisterButtonCallback(BTN_UP | BTN_DOWN | BTN_SELECT, ButtonShortPress, FieldPressHandler);
		RegisterButtonCallback(BTN_UP | BTN_DOWN, ButtonLongDown | ButtonLongPress, FieldLongPressHandler);
		break;

	case Normal:
	default:
		RegisterButtonCallback(BTN_SELECT, ButtonLongDown, ShowMenuHandler);
		break;
	}

	ClearScreen();
	TriggerRender();
}

void SetAlarmRing(PSong song)
{
	alarmRing = song;
}

void OnRtcSecond()
{
	GetTime(&clockValues);

	if(++clockSetValues.tm_sec >= 60)
		clockSetValues.tm_sec = 0;

	switch(clockState)
	{
	case Menu:
		break;
	case ClockSet:
		if(clockSetField == Second)
			TriggerRender();
		break;

	default:
		TriggerRender();
		break;
	}

	static int yday = -1;
	if(clockValues.tm_yday != yday)
	{
		yday = clockValues.tm_yday;

		specialDay 		= NULL;
		specialDayText	= NULL;

		for(int i=0; i<sizeof(specialDays) / sizeof(SpecialDay); i++)
		{
			struct tm* ptm = localtime(&specialDays[i].time);
			if(ptm->tm_mday == clockValues.tm_mday && ptm->tm_mon == clockValues.tm_mon)
			{
				specialDay 		= &specialDays[i];
				specialDayText	= (char*)*specialDay->texts;
				specialDayYears = clockValues.tm_year - ptm->tm_year;
				break;
			}
		}
	}
}

void OnRtcAlarm()
{
	// Ignore an alarm if the user is busy with the menu
	if(clockState != Menu)
		ChangeState(AlarmRing);
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


