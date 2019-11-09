/*
 * menu.c
 *
 *  Created on: 10/11/2019
 */

#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include "stm32f10x.h"
#include "rtc.h"
#include "macros.h"
#include "system.h"
#include "sound.h"
#include "song.h"
#include "graphics.h"
#include "fonts.h"
#include "clock.h"
#include "render.h"

MenuItem menu[] = {
	{ ClockSet,	"SET TIME" },
	{ DateSet,	"SET DATE" },
	{ AlarmSet, "SET ALARM" },
	{ AlarmToggle, "SET ALARM ON" },
	{ About, "ABOUT THIS CLOCK" },
	{ Normal, "EXIT" }
};

uint8_t menuIndex	= 0;

void MenuUp()
{
	if(menuIndex > 0)
	{
		menuIndex--;
		TriggerRender();
	}
}

void MenuDown()
{
	if(menuIndex < (sizeof(menu) / sizeof(MenuItem))-1)
	{
		menuIndex++;
		TriggerRender();
	}
}

void MenuSelect()
{
	ChangeState(menu[menuIndex].st);
}

void RenderMenu()
{
	ClearScreen();
	SetFont(sysFont);

	uint16_t top	= 40;
	for(uint8_t i = 0; i < sizeof(menu) / sizeof(MenuItem); i++)
	{
		SetForegroundColour((i == menuIndex) ? BLACK : GREEN);
		SetBackgroundColour((i == menuIndex) ? GREEN : BLACK);
		DrawText(10, top, menu[i].text);

		top += sysFont->height + 1;
	}
}
