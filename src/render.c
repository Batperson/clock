/*
 * render.c
 *
 *  Created on: 7/11/2019
 */
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "core_cm3.h"
#include "macros.h"
#include "system.h"
#include "graphics.h"
#include "sound.h"
#include "song.h"
#include "fonts.h"
#include "st7735.h"
#include "clock.h"
#include "menu.h"

typedef enum
{
	RenderNone				= 0x00,
	RenderTimeDate			= 0x01,
	RenderSpecialDayBanner	= 0x10,
	RenderAll				= 0xFFFF
} RenderPart;

static RenderPart	renderPart			= RenderAll;

static const char* szAlarmKeys[] 		= { "SEL", "UP", "DN" };

#define SPECIAL_DAY_TEXT_ROLL_SECS	6
#define SPECIAL_DAY_FADE_MSECS		120



void InitRender()
{
	// No need to use NVIC_Init to enable this system interrupt but we do want to set the priority.
	// We need to use this function for IRQs with negative values such as PendSV, SVC etc
	// We want PendSV to have the lowest possible priority so it doesn't pre-empt any time-critical interrupts
	NVIC_SetPriority(PendSV_IRQn, 255);

	printf("Rendering subsystem initialized\n");
}

void TriggerRender()
{
	renderPart	= RenderAll;
	SCB->ICSR |= SCB_ICSR_PENDSVSET;
}

static void ALWAYS_INLINE TriggerRenderPart(RenderPart part)
{
	renderPart |= part;
	SCB->ICSR |= SCB_ICSR_PENDSVSET;
}


const uint8_t 	maxIntensity 	= 16;
static uint8_t 	callbackCnt 	= 0;
static char szBanner[64];

void SpecialDayCallback()
{
	if(clockState == Normal)
		TriggerRenderPart(RenderSpecialDayBanner);

	if(callbackCnt++ == maxIntensity)
	{
		specialDayTextIndex++;
		if(specialDay->texts[specialDayTextIndex] == NULL)
			specialDayTextIndex = 0;

		sprintf(szBanner, specialDay->texts[specialDayTextIndex], specialDayYears);
	}

	if(callbackCnt >= (maxIntensity * 2))
	{
		callbackCnt = 0;

		DeregisterCallback(SpecialDayCallback);
	}
}

void RenderNormal()
{
	if(renderPart & RenderTimeDate)
	{
		char sz[24];
		if(mode & Mode24HourDisplay)
		{
			strftime(sz, sizeof(sz), "%H:%M", &clockValues);	// %H = 24 hour, %I = 12 hour, %p = AM/PM
		}
		else
		{
			strftime(sz, sizeof(sz), "%I:%M", &clockValues);
			strcat(sz, (clockValues.tm_hour >= 12) ? "<" : ";");
		}

		SetBackgroundColour(BLACK);
		SetForegroundColour(CYAN);
		SetFont(lcdFont);
		DrawText(0, 36, 160, 36, AlignCentre, sz);

		strftime(sz, sizeof(sz), "%a %d %b %Y", &clockValues);	// %a = Sun, %A = Sunday, %b = Jan, %B = January
		strupr(sz);

		SetForegroundColour(BLUE);
		SetFont(sysFont);
		DrawText(0, 80, 160, 12, AlignCentre, sz);

		if(specialDay != NULL && clockValues.tm_sec % SPECIAL_DAY_TEXT_ROLL_SECS == 0)
			RegisterTimeoutCallback(SpecialDayCallback, SPECIAL_DAY_FADE_MSECS, CallbackRepeat);
	}

	/*
	uint16_t top 	= 40;
	DrawRect(153, top, 8, clockValues.tm_sec, DrawNormal);
	DrawGradientVertical(153, top + clockValues.tm_sec, 8, 60 - clockValues.tm_sec, clockValues.tm_sec);
	*/

	if(renderPart & RenderSpecialDayBanner && specialDay != NULL)
	{
		SetBackgroundColour(BLACK);
		SetFont(sysFont);

		if(callbackCnt <= maxIntensity)
		{
			if(specialDayTextIndex >= 0)
			{
				SetForegroundColour(FadeColour(YELLOW, (maxIntensity - callbackCnt)));
				DrawText(0, 94, 160, 12, AlignCentre | AlignVCentre, szBanner);
			}
		}
		else
		{
			SetForegroundColour(FadeColour(YELLOW, (callbackCnt - maxIntensity)));
			DrawText(0, 94, 160, 12, AlignCentre | AlignVCentre, szBanner);
		}
	}

	renderPart = RenderNone;
}

void RenderAlarm()
{
	char sz[24];
	if(mode & Mode24HourDisplay)
	{
		strftime(sz, sizeof(sz), "%H:%M", &clockValues);	// %H = 24 hour, %I = 12 hour, %p = AM/PM
	}
	else
	{
		strftime(sz, sizeof(sz), "%I:%M", &clockValues);
		strcat(sz, (clockValues.tm_hour >= 12) ? "<" : ";");
	}

	uint8_t r		= 255; //rand() % 256;
	uint8_t g		= 0; //rand() % 256;
	uint8_t b		= 0; //rand() % 256;

	Colour fg		= RGB(r, g, b);
	SetBackgroundColour(BLACK);
	SetForegroundColour(fg);
	SetFont(lcdFont);

	DrawText(0, 36, 152, 36, AlignCentre, sz);

	strftime(sz, sizeof(sz), "%a %d %b %Y", &clockValues);	// %a = Sun, %A = Sunday, %b = Jan, %B = January
	strupr(sz);

	SetForegroundColour(YELLOW);
	SetFont(sysFont);
	DrawText(0, 80, 152, 12, AlignCentre, "TO STOP ALARM PRESS:");

	if(mode & ModeAlarmLock)
	{
		uint16_t l	= 4;
		for(int i=0; i<4; i++)
		{
			SetForegroundColour((i < alarmLockIndex) ? GREEN : RED);

			DrawText(l, 94, 34, 12, DrawInverse | FillMargin | AlignCentre | AlignVCentre, szAlarmKeys[alarmLock[i]]);

			l += 36;
		}
	}
}

void RenderFieldSet()
{
	char sz[24];

	switch(clockSetField)
	{
	case Hour:
		if(mode & Mode24HourDisplay)
		{
			strftime(sz, sizeof(sz), "%H", &clockSetValues);
		}
		else
		{
			strftime(sz, sizeof(sz), "%I", &clockSetValues);
			strcat(sz, (clockSetValues.tm_hour >= 12) ? "<" : ";");
		}
		break;
	case Minute:
		strftime(sz, sizeof(sz), "%M", &clockSetValues);
		break;
	case Second:
		strftime(sz, sizeof(sz), "%S", &clockSetValues);
		break;
	case Year:
		strftime(sz, sizeof(sz), "%Y", &clockSetValues);
		break;
	case Month:
		strftime(sz, sizeof(sz), "%m", &clockSetValues);	// %b = JAN, %B = January
		break;
	case Day:
		strftime(sz, sizeof(sz), "%d", &clockSetValues);
		break;
	default:
		break;
	}

	SetBackgroundColour(BLACK);
	SetForegroundColour(CYAN);
	SetFont(lcdFont);
	DrawText(0, 36, 152, 36, AlignCentre, sz);

	switch(clockSetField)
	{
	case Hour:
		strcpy(sz, "SET HOUR");
		break;
	case Minute:
		strcpy(sz, "SET MINUTE");
		break;
	case Second:
		strcpy(sz, "SET SECOND");
		break;
	case Year:
		strcpy(sz, "SET YEAR");
		break;
	case Month:
		strcpy(sz, "SET MONTH");
		break;
	case Day:
		strcpy(sz, "SET DAY");
		break;
	default:
		break;
	}

	SetForegroundColour(MAGENTA);
	SetFont(sysFont);
	DrawText(0, 80, 152, 12, AlignCentre, sz);
}

void RenderAbout()
{

}

extern void RenderMenu();

void Render()
{
	switch(clockState)
	{
	case ClockSet:
	case AlarmSet:
	case DateSet:
		RenderFieldSet();
		break;
	case Menu:
		RenderMenu();
		break;
	case About:
		RenderAbout();
		break;
	case AlarmRing:
		RenderAlarm();
		break;
	case Normal:
	default:
		RenderNormal();
		break;
	}
}

void INTERRUPT PendSV_Handler()
{
	Render();
}
