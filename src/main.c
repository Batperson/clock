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
const char* christmasTexts[]			= { "Merry Christmas!", NULL };
const char* newYearTexts[]				= { "Happy New Year!", NULL };
const char* waitangiDayTexts[]			= { "Today is Waitangi Day!", NULL };

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

void ChangeState(ClockState state)
{
	switch(clockState)
	{
	case AlarmRing:
		EndSong();
		break;

	case Menu:
		// We don't clear the screen when drawing the clock, but we do need to if we have been displaying a menu or some
		// other graphics.
		ClearScreen();
		TriggerRender();
		break;

	default:
		break;
	}

	clockState = state;

	ClearScreen();
	TriggerRender();

	switch(clockState)
	{
	case AlarmRing:
		SelectSong((specialDay != NULL && specialDay->specialSong != NULL) ? specialDay->specialSong : alarmRing);
		PlaySong();
		break;

	case AlarmSnooze:
		// TODO: Set the alarm to go again in n minutes
		break;

	case ClockSet:
		clockSetField = Hour;
		GetTime(&clockSetValues);
		break;

	case AlarmSet:
		clockSetField = Hour;
		GetAlarmTime(&clockSetValues);
		break;

	default:
		break;
	}
}

void SetAlarmRing(PSong song)
{
	alarmRing = song;
}

void ButtonSelectPress()
{
	switch(clockState)
	{
	case Menu:
		MenuSelect();
		break;
	case ClockSet:
		switch(clockSetField)
		{
		case Hour:
			clockSetField = Minute;
			break;
		case Minute:
			clockSetField = Second;
			ClearScreen();
			TriggerRender();
			break;
		case Second:
			SetTime(&clockSetValues);
			GetTime(&clockValues);
			ClearScreen();
			ChangeState(Normal);
			break;
		default:
			break;
		}
		TriggerRender();
		break;
	case DateSet:
		switch(clockSetField)
		{
		case Day:
			clockSetField = Month;
			break;
		case Month:
			clockSetField = Year;
			break;
		case Year:
			SetTime(&clockSetValues);
			GetTime(&clockValues);
			ClearScreen();
			ChangeState(Normal);
			break;
		default:
			break;
		}
		TriggerRender();
	case AlarmSet:
		switch(clockSetField)
		{
		case Hour:
			clockSetField = Minute;
			break;
		case Minute:
			SetAlarmTimeAndFlags(&clockSetValues, RecurWeekday | RecurWeekend);	// For now
			ClearScreen();
			ChangeState(Normal);
			break;
		default:
			break;
		}
		TriggerRender();
	default:
		break;
	}
}

void ButtonUpPress()
{
	switch(clockState)
	{
	case Menu:
		MenuUp();
		break;
	case ClockSet:
	case AlarmSet:
		switch(clockSetField)
		{
		case Hour:
			if(--clockSetValues.tm_hour < 0) clockSetValues.tm_hour = 23;
			break;
		case Minute:
			if(--clockSetValues.tm_min < 0) clockSetValues.tm_min = 59;
			break;
		case Second:
			clockSetValues.tm_sec = 0;
			break;
		default:
			break;
		}
		TriggerRender();
		break;
	default:
		break;
	}
}

void ButtonDownPress()
{
	switch(clockState)
	{
	case Menu:
		MenuDown();
		break;
	case ClockSet:
	case AlarmSet:
		switch(clockSetField)
		{
		case Hour:
			if(++clockSetValues.tm_hour > 23) clockSetValues.tm_hour = 0;
			break;
		case Minute:
			if(++clockSetValues.tm_min > 59) clockSetValues.tm_min = 0;
			break;
		case Second:
			clockSetValues.tm_sec = 0;
			break;
		default:
			break;
		}
		TriggerRender();
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
		SelectSong((PSong)item->arg);
		PlaySong();
	}
}

void RTC_OnSecond()
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

void RTC_OnAlarm()
{
	// Ignore an alarm if the user is busy with the menu
	if(clockState != Menu) ChangeState(AlarmRing);
}

void BTN_OnDown(uint32_t btn)
{
	if(clockState == AlarmRing && !(btn & BUTTON_LPRESS))
	{
		ChangeState(AlarmSnooze);
	}
	else if(btn == (BUTTON_LPRESS | BTN_SELECT))
	{
		Beep(88000, 100, 90);
		ChangeState(Menu);
	}
}

void BTN_OnPress(uint32_t btn)
{
	if(!(btn & BUTTON_LPRESS))
	{
		switch(btn)
		{
		case BTN_SELECT:
			ButtonSelectPress();
			break;
		case BTN_UP:
			ButtonUpPress();
			break;
		case BTN_DOWN:
			ButtonDownPress();
			break;
		default:
			break;
		}

		Beep(88000, 60, 90);
	}
}

