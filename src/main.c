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
#include "st7735.h"

extern Voice sine;

int main(void)
{
	printf("STM32F103 RTC Alarm Clock v0.01 by Weka Workshop\n");

	InitClock();
	InitSystem();
	InitDisplay();
	InitSound();

	ClearScreen();

	//SelectVoice(0, 255 , &sine);
	//SelectVoice(1, 255 , &sine);
	//SoundOn(0, 44000);
	//SoundOn(1, 52325);

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
