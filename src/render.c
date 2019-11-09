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

void InitRender()
{
	// No need to use NVIC_Init to enable this system interrupt but we do want to set the priority.
	// We need to use this function for IRQs with negative values such as PendSV, SVC etc
	// We want PendSV to have the lowest possible priority so it doesn't pre-empt any time-critical interrupts, especially sync
	NVIC_SetPriority(PendSV_IRQn, 255);

	printf("Rendering subsystem initialized");
}

void TriggerRender()
{
	SCB->ICSR |= SCB_ICSR_PENDSVSET;
}

void RenderNormal()
{
	char sz[24];
	strftime(sz, sizeof(sz), "%I:%M", &clockFields);

	SetBackgroundColour(BLACK);
	SetForegroundColour(CYAN);
	SetFont(lcdFont);
	DrawText(10, 36, sz);

	strftime(sz, sizeof(sz), "%a %d %b %Y", &clockFields);
	strupr(sz);

	SetForegroundColour(BLUE);
	SetFont(sysFont);
	DrawText(10, 74, sz);

	uint16_t top 	= 40;
	SetForegroundColour(BLACK);
	DrawRect(153, top, 8, clockFields.tm_sec);
	DrawGradientVertical(153, top + clockFields.tm_sec, 8, 60 - clockFields.tm_sec, clockFields.tm_sec);
}

void RenderClockSet()
{

}

void RenderAlarmSet()
{

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
		RenderClockSet();
		break;
	case AlarmSet:
		RenderAlarmSet();
		break;
	case Menu:
		RenderMenu();
		break;
	case About:
		RenderAbout();
		break;
	case Normal:
	case AlarmRing:
	case AlarmSnooze:
	default:
		RenderNormal();
		break;
	}
}

void INTERRUPT PendSV_Handler()
{
	Render();
}
