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

	ClearScreen();

	SelectSong(&arpegios);
	PlaySong();

	while(1)
	{
		/* Loop infinitely */
	}
}

void RTC_OnSecond()
{
	struct tm tm;
	GetTime(&tm);

	char sz[24];
	strftime(sz, sizeof(sz), "%I:%M", &tm);

	SetForegroundColour(CYAN);
	SetFont(lcdFont);
	DrawText(10, 36, sz);

	strftime(sz, sizeof(sz), "%a %d %b %Y", &tm);
	strupr(sz);

	SetForegroundColour(BLUE);
	SetFont(sysFont);
	DrawText(10, 74, sz);

	uint16_t top 	= 40;
	SetForegroundColour(BLACK);
	DrawRect(153, top, 8, tm.tm_sec);
	DrawGradientVertical(153, top + tm.tm_sec, 8, 60 - tm.tm_sec, tm.tm_sec);
}
