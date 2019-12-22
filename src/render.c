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
#include "macros.h"
#include "system.h"
#include "graphics.h"
#include "sound.h"
#include "song.h"
#include "fonts.h"
#include "st7735.h"
#include "clock.h"
#include "menu.h"

static const char* szAlarmKeys[] 		= { "SEL", "UP", "DN" };

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
	SCB->ICSR |= SCB_ICSR_PENDSVSET;
}

void RenderTextBanner(char* sz, Colour fg, uint8_t intensity)
{
	uint32_t 		r	= (fg >> 11) 	& 0x1F;
	uint32_t		g	= (fg >> 5)  	& 0x3F;
	uint32_t		b	= fg 			& 0x1F;

	// intensity = 0 - 8
	r 	= (r * intensity) / 8;
	g 	= (g * intensity) / 8;
	b 	= (b * intensity) / 8;

	SetForegroundColour(RGB(r, g, b));
	SetBackgroundColour(BLACK);
	SetFont(sysFont);
	DrawText(0, 94, 162, 12, AlignCentre | AlignVCentre, sz);
}


void SpecialDayCallback()
{
	const uint8_t 	maxIntensity 	= 8;
	static uint8_t 	callbackCnt 	= 0;
	static char sz[64];

	if(callbackCnt <= maxIntensity)
	{
		if(specialDayTextIndex >= 0)
			RenderTextBanner(sz, YELLOW, (maxIntensity - callbackCnt));

		if(callbackCnt == maxIntensity)
		{
			specialDayTextIndex++;
			if(specialDay->texts[specialDayTextIndex] == NULL)
				specialDayTextIndex = 0;

			sprintf(sz, specialDay->texts[specialDayTextIndex], specialDayYears);
		}
	}
	else
	{
		RenderTextBanner(sz, YELLOW, (callbackCnt - maxIntensity));
	}

	if(callbackCnt++ >= (maxIntensity * 2))
	{
		callbackCnt = 0;

		DeregisterCallback(SpecialDayCallback);
	}
}

void RenderNormal()
{
	char sz[24];
	strftime(sz, sizeof(sz), "%I:%M", &clockValues);	// %H = 12 hour, %I = 24 hour, %p = AM/PM

	SetBackgroundColour(BLACK);
	SetForegroundColour(CYAN);
	SetFont(lcdFont);
	DrawText(0, 36, 152, 36, AlignCentre, sz);

	strftime(sz, sizeof(sz), "%a %d %b %Y", &clockValues);	// %a = Sun, %A = Sunday, %b = Jan, %B = January
	strupr(sz);

	SetForegroundColour(BLUE);
	SetFont(sysFont);
	DrawText(0, 80, 152, 12, AlignCentre, sz);

	uint16_t top 	= 40;
	SetForegroundColour(BLACK);
	DrawRect(153, top, 8, clockValues.tm_sec, DrawNormal);
	DrawGradientVertical(153, top + clockValues.tm_sec, 8, 60 - clockValues.tm_sec, clockValues.tm_sec);

	if(specialDay != NULL && clockValues.tm_min & 0x04)
	{
		SetForegroundColour(BLACK);
		DrawRect(0, 94, 162, 12, DrawNormal);

		RegisterTimeoutCallback(SpecialDayCallback, 20, CallbackRepeat);
	}
}

void RenderAlarm()
{
	char sz[24];
	strftime(sz, sizeof(sz), "%I:%M", &clockValues);	// %H = 12 hour, %I = 24 hour, %p = AM/PM

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
		strftime(sz, sizeof(sz), "%I", &clockSetValues);
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
