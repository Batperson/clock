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
#include "st7735.h"

int main(void)
{
	printf("Weka RTC Alarm Clock v0.01\n");

	InitClock();
	InitSystem();
	InitDisplay();

	ClearScreen();

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
