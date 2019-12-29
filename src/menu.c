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
#include "menu.h"

PMenuItem 	currentMenu	= NULL;
uint8_t 	menuIndex	= 0;
uint8_t 	menuLength	= 0;

void SetCurrentMenu(PMenuItem menu)
{
	menuIndex	= 0;
	currentMenu = menu;

	uint8_t i = 0;
	while(1)
	{
		if(menu[i].text == NULL)
		{
			menuLength = i;
			break;
		}

		i++;
	}

	if(clockState == Menu) TriggerRender();
}

void WEAKREF OnMenuTimeout(PMenuItem item)
{

}

void WEAKREF OnMenuHighlight(PMenuItem item)
{

}

void MenuTimeoutHandler()
{
	OnMenuTimeout(&currentMenu[menuIndex]);
}

void MenuUp()
{
	if(menuIndex > 0)
	{
		menuIndex--;
		RegisterTimeoutCallback(MenuTimeoutHandler, MENU_TIMEOUT_MILLIS, CallbackNone);
		OnMenuHighlight(&currentMenu[menuIndex]);

		TriggerRender();
	}
}

void MenuDown()
{
	if(menuIndex < menuLength - 1)
	{
		menuIndex++;
		RegisterTimeoutCallback(MenuTimeoutHandler, MENU_TIMEOUT_MILLIS, CallbackNone);
		OnMenuHighlight(&currentMenu[menuIndex]);

		TriggerRender();
	}
}

void MenuSelect()
{
	PMenuProc menuProc	= (PMenuProc)currentMenu[menuIndex].proc;
	uint32_t arg		= currentMenu[menuIndex].arg;

	DeregisterCallback(MenuTimeoutHandler);

	if(menuProc)
		menuProc(arg);
}

void RenderMenu()
{
	uint16_t w, h, dw, dh;

	ClearScreen();
	SetFont(sysFont);
	MeasureChar(' ', &w, &h);
	MeasureDisplay(&dw, &dh);

	uint16_t top	= h;
	uint16_t left	= w;
	uint16_t width	= dw - (2 * w);
	uint8_t start;
	uint8_t end;

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
		DrawText(left, top, width, h, (i == menuIndex) ? DrawInverse : DrawNormal, currentMenu[i].text);

		top += h + 1;
	}
}
