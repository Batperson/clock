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
#include "render.h"


MenuItem menu[] = {
	{ ClockSet,	"Set Time" },
	{ DateSet,	"Set Date" },
	{ AlarmSet, "Set Alarm" },
	{ AlarmToggle, "Alarm On" },
	{ About, "About" },
	{ Normal, "Exit" }
};

const char* birthdayChristopherTexts[] 	= { "Happy birthday Christopher!", "You are now %d years old!", "Hope you have a really nice day today", 0 };
const char* birthdayRosieTexts[]		= { "Today is Rosie's birthday!", "She is %d today!", "Be really nice to her on her special day!", 0 };
const char* birthdayXiaTexts[]			= { "It's Mum's birthday!", "She is %d today.", "Do something nice for her!", 0 };
const char* birthdayPeterTexts[]		= { "It's Dad's birthday!", "He is %d today.", "Give him a handshake!", 0 };
const char* christmasTexts[]			= { "Merry Christmas!", 0 };
const char* newYearTexts[]				= { "Happy New Year!", 0 };

SpecialDay specialDays[] = {
	{ 65535, 0, birthdayChristopherTexts },
	{ 65535, 0, birthdayRosieTexts },
	{ 65535, 0, birthdayXiaTexts },
	{ 65535, 0, birthdayPeterTexts },
	{ 65535, 0, christmasTexts },
	{ 65535, 0, newYearTexts },
	{ 0, 0, 0 }
};

ClockState 		clockState 			= Normal;
ClockSetField 	clockField 			= Hour;
AlarmState		alarmState			= Disabled;
uint8_t 		quarterSecond		= -1;
uint8_t			specialDay			= -1;
struct tm 		clockFields;

extern Song arpegios;
extern Song reveille;

// 	Pins
//	A9				Button (Select)
//	A8				Audio
//	A7				Screen (MOSI)
//	A6				Screen (Data/Command)
//	A5				Screen (SCK)
//	A4				Screen (Slave select)
//	A3				Screen (Reset)
// 	A2				Screen (Backlight)
//	A1				Button (Up)
//	A0				Button (Down)

int main(void)
{
	printf("STM32F103 RTC Alarm Clock v0.01 by Weka Workshop\n");

	InitClock();
	InitSystem();
	InitDisplay();
	InitSound();
	InitSong();
	InitButton();

	GetTime(&clockFields);
	ClearScreen();

	//SelectSong(&reveille);
	//PlaySong();

	while(1)
	{
		// Maybe this will save some milliamps?
		__WFI();
	}
}

void ToggleAlarm()
{
	if(alarmState == Enabled)
	{
		alarmState = Disabled;
	}
	else
	{
		alarmState = Enabled;
	}

	clockState = Normal;
}

void ChangeState(ClockState state)
{
	switch(state)
	{
	case AlarmRing:
	case AlarmSnooze:
		EndSong();
		break;

	default:break;
	}

	clockState = state;

	switch(clockState)
	{
	case AlarmToggle:
		ToggleAlarm();
		break;

	default:
		break;
	}
}

void RTC_OnSecond()
{
	GetTime(&clockFields);
	Render();

	/*
	if(quarterSecond < 0)
	{

	}
	else
	{
		if(++quarterSecond > 3)
		{
			quarterSecond = 0;

			GetTime(&clockFields);
		}

		Render();
	}
	*/
}

void RTC_OnAlarm()
{
	ChangeState(AlarmRing);
}

void BTN_OnDown(uint16_t btn, PressType press)
{
	if(clockState == AlarmRing && press == Short)
	{
		ChangeState(AlarmSnooze);
	}
	else if(press == Long)
	{
		Beep(88000, 100, 90);
		ChangeState(Menu);
	}
}

void BTN_OnPress(uint16_t btn, PressType press)
{
	if(press == Short)
		Beep(88000, 60, 90);
}

