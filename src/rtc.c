/*
 * rtc.c
 *
 *  Created on: 21/09/2019
 */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "stm32f10x.h"
#include "macros.h"
#include "rtc.h"

#define BREG_ONETIMECONFIG			BKP_DR1
#define BREG_ALARM					BKP_DR2

void InitRTCInterrupts()
{
	NVIC_InitTypeDef nvic;

	/* Enable the RTC Interrupt */
	nvic.NVIC_IRQChannel 					= RTC_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority 	= 4;
	nvic.NVIC_IRQChannelSubPriority 		= 0;
	nvic.NVIC_IRQChannelCmd 				= ENABLE;
	NVIC_Init(&nvic);
}

void InitRTCOneTimeConfig(void)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);

	/* Reset Backup Domain */
	BKP_DeInit();

	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}

	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Enable the RTC Second and Alarm interrupts */
	RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR, ENABLE);

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

void ConfigAlarm(PAlarm alm)
{
	if(alm->flags & (RecurWeekday | RecurWeekend))
	{
		struct tm at;
		GetTime(&at);

		at.tm_hour	= alm->hour;
		at.tm_min	= alm->minute;
		at.tm_sec	= 0;

		// Calculate the alarm time based on today, with the hours and minutes specified
		time_t atim	= mktime(&at);
		time_t ctim = (time_t)RTC_GetCounter();

		// If the alarm time is already in the past, add 24 hours
		if(atim <= ctim)
			atim += 24 * 3600;

		struct tm* pat = localtime(&atim);

		while(1)
		{
			// Keep adding 24 hours until we get a time which complies with the recurrence flag
			if(alm->flags & RecurWeekday && pat->tm_wday > 0 && pat->tm_wday < 6)
				break;
			if(alm->flags & RecurWeekend && (pat->tm_wday < 1 || pat->tm_wday > 5))
				break;

			atim += 24 * 3600;
			pat = localtime(&atim);
		}

		RTC_SetAlarm(atim);

		char sz[32];
		strftime(sz, sizeof(sz), "Next alarm: %F %R\n", pat);
		printf(sz);
	}
	else
	{
		RTC_SetAlarm(-1);

		printf("Alarm disabled\n");
	}
}

void ConfigNextAlarm()
{
	Alarm al;
	GetAlarm(&al);
	ConfigAlarm(&al);
}

void InitClock()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	PWR_BackupAccessCmd(ENABLE);

	InitRTCInterrupts();

	/* Check for a flag in the backup register which should already be set if the RTC
	 * has been programmed previously */
	if (BKP_ReadBackupRegister(BREG_ONETIMECONFIG) != 0xA5A5)
	{
		/* RTC has not been previously configured */
		printf("Initializing RTC and backup domain\n");

	    InitRTCOneTimeConfig();

	    BKP_WriteBackupRegister(BREG_ONETIMECONFIG, 0xA5A5);
	    BKP_WriteBackupRegister(BREG_ALARM, 0);
	}
	else
	{
		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Enable the RTC Second and Alarm interrupts */
		RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR, ENABLE);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}

	/* Clear reset flags */
	RCC_ClearFlag();

	ConfigNextAlarm();

	printf("Clock initialized\n");
}

void SetTime(struct tm* ptm)
{
	time_t tim = mktime(ptm);

	RTC_SetCounter(tim);
}

void GetTime(struct tm* ptm)
{
	time_t tim = (time_t)RTC_GetCounter();

	struct tm* pltm = localtime(&tim);
	memcpy(ptm, pltm, sizeof(struct tm));
}

void SetAlarm(PAlarm alm)
{
	uint16_t val	= (alm->minute & 0x3F);
	val 			|= (alm->hour << 6) & 0x7C0;
	val				|= (alm->flags & AlarmFlagsMask);

	BKP_WriteBackupRegister(BREG_ALARM, val);

	ConfigAlarm(alm);
}

void GetAlarm(PAlarm alm)
{
	uint16_t val	= BKP_ReadBackupRegister(BREG_ALARM);

	alm->flags		= val & AlarmFlagsMask;
	alm->minute		= val & 0x3F;
	alm->hour		= (val & 0x7C0) >> 6;
}

void SnoozeAlarm(uint8_t minutes)
{
	time_t tim		= RTC_GetCounter();
	tim 			+= (60 * minutes);

	RTC_SetAlarm(tim);
}

void RTC_IRQHandler()
{
	int it = RTC->CRH & (RTC_IT_SEC | RTC_IT_ALR);
	RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_ALR);

	if(it & RTC_IT_SEC)
		RTC_OnSecond();

	if(it & RTC_IT_ALR)
	{
		printf("Alarm activated\n");

		ConfigNextAlarm();

		RTC_OnAlarm();
	}
}

void WEAKREF RTC_OnSecond()
{

}

void WEAKREF RTC_OnAlarm()
{

}

