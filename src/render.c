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
#include "icons.h"
#include "st7735.h"
#include "clock.h"
#include "menu.h"

extern Bitmap* about;

typedef enum
{
	RenderNone				= 0x0000,
	RenderTimeDate			= 0x0001,
	RenderSpecialDayBanner	= 0x0010,
	Refresh					= 0x8000,
	RenderAll				= 0x7FFF
} RenderPart;

static RenderPart	renderPart			= RenderAll;

static const char* szAlarmKeys[] 		= { "SEL", "UP", "DN" };

static const Colour alarmStripes[]		= { RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA };

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

void TriggerRenderRefresh()
{
	renderPart |= Refresh | RenderAll;
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
	if(clockState == Normal && specialDayState == SpecialDayShow)
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

		RemoveBrush();
		SetBackgroundColour(BLACK);
		SetForegroundColour(CYAN);
		SetFont(lcdFont);

		PBitmap bm = bellSlash;
		if(alarmState & AlarmStateSnoozed)
			bm = bellSnooze;
		else if(mode & ModeAlarm)
			bm = bell;

#ifdef SCREEN_MISALIGNED
		DrawBitmap(46, 12, DrawNormal, bm);
		DrawText(10, 36, 150, 36, AlignCentre, sz);
#else
		DrawBitmap(36, 12, DrawNormal, bm);
		DrawText(0, 36, 160, 36, AlignCentre, sz);
#endif

		strftime(sz, sizeof(sz), "%a %d %b %Y", &clockValues);	// %a = Sun, %A = Sunday, %b = Jan, %B = January
		strupr(sz);

		SetFont(sysFont);
		DrawText(0, 80, 160, 12, AlignCentre, sz);

		if(specialDayState == SpecialDayShow && clockValues.tm_sec % SPECIAL_DAY_TEXT_ROLL_SECS == 0)
			RegisterTimeoutCallback(SpecialDayCallback, SPECIAL_DAY_FADE_MSECS, CallbackRepeat);
	}

	if(renderPart & RenderSpecialDayBanner && specialDayState == SpecialDayShow)
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

	static uint16_t stripeOffset = 0;

	SetBackgroundColour(BLACK);
	SetStripeBrush(alarmStripes, sizeof(alarmStripes) / sizeof(alarmStripes[0]), stripeOffset++, 4, Horizontal);
	SetFont(lcdFont);

#ifdef SCREEN_MISALIGNED
	DrawText(10, 36, 150, 36, AlignCentre, sz);
#else
	DrawText(0, 36, 160, 36, AlignCentre, sz);
#endif

	RemoveBrush();
	SetForegroundColour(YELLOW);
	SetFont(sysFont);

	if(((mode & ModeAlarmSnooze) == ModeAlarmSnooze) && (alarmState & AlarmStateSnoozed) == AlarmStateNone)
		DrawText(4, 80, 160, 12, AlignCentre, "LONG PRESS TO SNOOZE");

	if((mode & ModeAlarmLock) == ModeAlarmLock)
		DrawText(4, 90, 150, 12, AlignCentre, "TO RESET PRESS");
	else if((mode & ModeAlarmSnooze) == ModeAlarmSnooze)
		DrawText(4, 90, 150, 12, AlignCentre, "PRESS 3X TO RESET");
	else
		DrawText(4, 90, 150, 12, AlignCentre, "PRESS ANY TO RESET");

	if(mode & ModeAlarmLock)
	{
#ifdef SCREEN_MISALIGNED
		uint16_t l	= 10;
#else
		uint16_t l	= 4;
#endif

		for(int i=0; i<4; i++)
		{
			SetForegroundColour((i < alarmLockIndex) ? GREEN : RED);

			DrawText(l, 104, 34, 12, DrawInverse | FillMargin | AlignCentre | AlignVCentre, szAlarmKeys[alarmLock[i]]);

			l += 36;
		}
	}
}

void RenderTextDemo()
{
	sprintf(szBanner, specialDay->texts[specialDayTextIndex], specialDayYears);

	SetBackgroundColour(BLACK);
	SetFont(sysFont);

	SetForegroundColour(YELLOW);
	DrawText(0, 94, 160, 12, AlignCentre | AlignVCentre, szBanner);
}

void RenderFieldSet()
{
	char sz[24];

	switch(clockSetField)
	{
	case ClockHour:
	case AlarmHour:
	case NighttimeHoursStart:
	case NighttimeHoursEnd:
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
	case ClockMinute:
	case AlarmMinute:
		strftime(sz, sizeof(sz), "%M", &clockSetValues);
		break;
	case ClockSecond:
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
	case SnoozeMinutes:
		sprintf(sz, "%02d", clockSetValues.tm_yday);
		break;
	case RtcTrim:
		sprintf(sz, "%03d", clockSetValues.tm_yday);
		break;
	case DaytimeBrightness:
		sprintf(sz, "%02d", brightnessSettings.bs.daytimeBrightness);
		break;
	case NighttimeBrightness:
		sprintf(sz, "%02d", brightnessSettings.bs.nighttimeBrightness);
		break;
	default:
		break;
	}

	RemoveBrush();
	SetBackgroundColour(BLACK);
	SetForegroundColour(CYAN);
	SetFont(lcdFont);

#ifdef SCREEN_MISALIGNED
	DrawText(6, 36, 152, 36, AlignCentre | FillMargin, sz);
#else
	DrawText(0, 36, 152, 36, AlignCentre | FillMargin, sz);
#endif

	switch(clockSetField)
	{
	case ClockHour:
	case AlarmHour:
		strcpy(sz, "SET HOUR");
		break;
	case ClockMinute:
	case AlarmMinute:
		strcpy(sz, "SET MINUTE");
		break;
	case ClockSecond:
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
	case SnoozeMinutes:
		strcpy(sz, "SET SNOOZE MINS");
		break;
	case RtcTrim:
		strcpy(sz, "SET OSC CALIB VALUE");
		break;
	case DaytimeBrightness:
		strcpy(sz, "SET DAY BRIGHTNESS");
		break;
	case NighttimeBrightness:
		strcpy(sz, "SET NIGHT BRIGHTNESS");
		break;
	case NighttimeHoursStart:
		strcpy(sz, "SET NIGHT START");
		break;
	case NighttimeHoursEnd:
		strcpy(sz, "SET NIGHT END");
		break;
	default:
		break;
	}

	SetForegroundColour(MAGENTA);
	SetFont(sysFont);

#ifdef SCREEN_MISALIGNED
	DrawText(6, 80, 152, 12, AlignCentre | FillMargin, sz);
#else
	DrawText(0, 80, 152, 12, AlignCentre | FillMargin, sz);
#endif
}

void RenderAbout()
{
	static uint16_t offs = 300;

	SetForegroundColour(WHITE);
	SetBackgroundColour(BLACK);
	SetGradientBrush(300, offs--, Vertical);

#ifdef SCREEN_MISALIGNED
	DrawBitmap(10, 0, DrawNormal, about);
#else
	DrawBitmap(10, 6, DrawNormal, about);
#endif

	if(offs == 0)
		offs = 300;

	SetFont(sysFont);
#ifdef SCREEN_MISALIGNED
	DrawText(0, 114, 160, 12, AlignCentre, "FOR CHRISTOPHER");
#elif ROSIE_BUILD
	DrawText(0, 120, 160, 12, AlignCentre, "FOR ROSIE");
#endif
}

void RenderMenu()
{
	uint16_t w, h, dw, dh;

	RemoveBrush();
	SetFont(sysFont);
	MeasureChar(' ', &w, &h);
	MeasureDisplay(&dw, &dh);

#ifdef SCREEN_MISALIGNED
	uint16_t top	= 0;
#else
	uint16_t top	= h;
#endif

	uint16_t left	= w;
	uint16_t width	= dw - (2 * w);
	uint8_t start;
	uint8_t end;
	uint8_t menuIndex;
	uint8_t menuLength;
	PMenuItem currentMenu;

	GetMenuState(&menuIndex, &menuLength, &currentMenu);

	if(menuIndex < (MAX_RENDER_MENU_ITEMS / 2))
		start = 0;
	else if(menuLength > MAX_RENDER_MENU_ITEMS && menuIndex > menuLength - MAX_RENDER_MENU_ITEMS)
		start = menuLength - MAX_RENDER_MENU_ITEMS;
	else
		start = menuIndex - (MAX_RENDER_MENU_ITEMS / 2);

	end = (start + MAX_RENDER_MENU_ITEMS > menuLength) ? menuLength : start + MAX_RENDER_MENU_ITEMS;

	for(uint8_t i = start; i < end; i++)
	{
		SetForegroundColour(GREEN);
		SetBackgroundColour(BLACK);
		DrawText(left, top, w, h, FillMargin | ((i == menuIndex) ? DrawInverse : DrawNormal), (currentMenu[i].flags & MenuSelected) ? "*" : " ");
		DrawText(left + w, top, width - w, h, FillMargin | ((i == menuIndex) ? DrawInverse : DrawNormal), currentMenu[i].text);

		top += h + 1;
	}
}

void Render()
{
	if(renderPart & Refresh)
	{
		renderPart &= ~Refresh;
		ClearScreen();
	}

	switch(clockState)
	{
	case BootStrap:
		break;
	case FieldSet:
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
	case TextDemo:
		RenderTextDemo();
		break;
	case Normal:
	default:
		RenderNormal();
		break;
	}

	renderPart |= RenderAll;
}

void INTERRUPT PendSV_Handler()
{
	// While we are rendering, disable other interrupts as we don't want to be pre-empted while in the middle of a
	// write to the TFT which will leave garbage on the screen or worse.
	//__disable_irq();

	Render();

	//__enable_irq();
}
