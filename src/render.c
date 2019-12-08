/*
 * render.c
 *
 *  Created on: 7/11/2019
 */
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
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
static const int16_t	lOffsets[4]		= { 0, 6, -4, 3 };
static const int16_t	tOffsets[4]		= { -5, 3, -0, -3 };
static uint8_t	alarmTick				= 0;

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
}

void RenderAlarm()
{
	char sz[24];
	strftime(sz, sizeof(sz), "%I:%M", &clockValues);	// %H = 12 hour, %I = 24 hour, %p = AM/PM

	uint8_t ix		= alarmTick++ & 0x03;
	uint8_t	lOff 	= lOffsets[ix];
	uint8_t tOff 	= tOffsets[ix];

	SetForegroundColour(BLACK);
	DrawRect(0, 0, 162, 74, DrawNormal);

	Colour fg		= CYAN;
	SetBackgroundColour(BLACK);
	SetForegroundColour(fg);
	SetFont(lcdFont);

	DrawText(10 + lOff, 36 + tOff, 142, 36, AlignCentre, sz);

	strftime(sz, sizeof(sz), "%a %d %b %Y", &clockValues);	// %a = Sun, %A = Sunday, %b = Jan, %B = January
	strupr(sz);

	SetForegroundColour(YELLOW);
	SetFont(sysFont);
	DrawText(0, 80, 152, 12, AlignCentre, "TO STOP ALARM PRESS:");

	if(alarmMode & AlarmLock)
	{
		uint16_t l	= 4;
		for(int i=0; i<4; i++)
		{
			SetForegroundColour((i < alarmLockIndex) ? GREEN : RED);
			DrawRect(l, 92, 34, 12, DrawNormal);
			DrawText(l, 92, 34, 12, DrawInverse | AlignCentre | AlignVCentre, szAlarmKeys[alarmLock[i]]);

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
