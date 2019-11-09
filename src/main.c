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

	InitClock();
	InitSystem();
	InitDisplay();
	InitSound();
	InitSong();
	InitButton();
	InitRender();

	ClearScreen();

	LoadConfiguration();

	//SelectSong(&reveille);
	//PlaySong();

	while(1)
	{
		// Stop the CPU clock until there is something to do
		// Maybe this will save some milliamps?
		__WFI();
	}
}

void SetAlarmState(AlarmState als)
{
	if(als == Enabled)
	{
		alarmState 		= Enabled;
		menu[3].text	= "SET ALARM OFF";
	}
	else
	{
		alarmState 		= Disabled;
		menu[3].text	= "SET ALARM ON";
	}
}

void ChangeState(ClockState state)
{
	switch(clockState)
	{
	case AlarmRing:
	case AlarmSnooze:
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

	switch(clockState)
	{
	case AlarmToggle:
		SetAlarmState(alarmState == Enabled ? Disabled : Enabled);
		TriggerRender();

		clockState = Menu;
		break;

	case Menu:
		TriggerRender();
		break;

	default:
		break;
	}
}

void RTC_OnSecond()
{
	GetTime(&clockFields);

	switch(clockState)
	{
	case Menu:
		break;

	default:
		TriggerRender();
		break;
	}
}

void RTC_OnAlarm()
{
	ChangeState(AlarmRing);
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
			if(clockState == Menu) MenuSelect();
			break;
		case BTN_UP:
			if(clockState == Menu) MenuUp();
			break;
		case BTN_DOWN:
			if(clockState == Menu) MenuDown();
			break;
		default:
			break;
		}

		Beep(88000, 60, 90);
	}
}

