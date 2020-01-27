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

void GetMenuState(uint8_t* pmenuIndex, uint8_t* pmenuLength, PMenuItem* pcurrentMenu)
{
	*pmenuIndex		= menuIndex;
	*pmenuLength	= menuLength;
	*pcurrentMenu	= currentMenu;
}

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
		DeregisterCallback(MenuTimeoutHandler);
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
		DeregisterCallback(MenuTimeoutHandler);
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
