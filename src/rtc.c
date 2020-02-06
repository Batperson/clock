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

#define RTC_ONETIME_VAL				0xA5A5

#define ALARM_MIN_MASK				0x03F
#define ALARM_HR_MASK				0x7C0
#define ALARM_HR_SHIFT				6

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

	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

void SetNextAlarm()
{
	uint16_t val			= BKP_ReadBackupRegister(BREG_ALARM);

	AlarmFlags flags		= val & AlarmFlagsMask;
	uint8_t min				= val & ALARM_MIN_MASK;
	uint8_t hour			= (val & ALARM_HR_MASK) >> ALARM_HR_SHIFT;

	if(flags & (RecurWeekday | RecurWeekend))
	{
		struct tm at;
		GetTime(&at);

		at.tm_hour	= hour;
		at.tm_min	= min;
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
			if(flags & RecurWeekday && pat->tm_wday > 0 && pat->tm_wday < 6)
				break;
			if(flags & RecurWeekend && (pat->tm_wday < 1 || pat->tm_wday > 5))
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

void WEAKREF OnInitBackupDomain()
{

}

void InitClock()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	PWR_BackupAccessCmd(ENABLE);

	/* Check for a flag in the backup register which should already be set if the RTC
	 * has been programmed previously */
	if (BKP_ReadBackupRegister(BREG_ONETIMECONFIG) != RTC_ONETIME_VAL)
	{
		/* RTC has not been previously configured */
		printf("Initializing RTC and backup domain\n");

	    InitRTCOneTimeConfig();
	    OnInitBackupDomain();

	    BKP_WriteBackupRegister(BREG_ONETIMECONFIG, RTC_ONETIME_VAL);
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

	/* Clear interrupt flags, then enable interrupts */
	RTC_ClearFlag(RTC_IT_SEC | RTC_IT_ALR | RTC_IT_OW);
	RTC_ITConfig(RTC_IT_SEC | RTC_IT_ALR, ENABLE);
	InitRTCInterrupts();

	/* Clear reset flags */
	RCC_ClearFlag();

	SetNextAlarm();

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

void GetAlarmTime(struct tm* ptm)
{
	memset(ptm, 0, sizeof(struct tm));

	uint16_t val	= BKP_ReadBackupRegister(BREG_ALARM);

	ptm->tm_min		= val & ALARM_MIN_MASK;
	ptm->tm_hour	= (val & ALARM_HR_MASK) >> 6;
}

void SetAlarmTime(struct tm* ptm)
{
	uint16_t val		= BKP_ReadBackupRegister(BREG_ALARM) & AlarmFlagsMask;

	if(ptm)
	{
		val				|= (ptm->tm_min & ALARM_MIN_MASK);
		val 			|= (ptm->tm_hour << ALARM_HR_SHIFT) & ALARM_HR_MASK;
	}

	BKP_WriteBackupRegister(BREG_ALARM, val);

	SetNextAlarm();
}

void SetAlarmFlags(AlarmFlags flags)
{
	uint16_t val	= BKP_ReadBackupRegister(BREG_ALARM);
	val 			&= (ALARM_HR_MASK | ALARM_MIN_MASK);
	val				|= flags;

	BKP_WriteBackupRegister(BREG_ALARM, val);

	SetNextAlarm();
}

AlarmFlags GetAlarmFlags()
{
	return BKP_ReadBackupRegister(BREG_ALARM) & AlarmFlagsMask;
}

void SnoozeAlarm(uint8_t minutes)
{
	time_t tim		= RTC_GetCounter();
	tim 			+= (60 * minutes);

	RTC_SetAlarm(tim);
}

void RTC_IRQHandler()
{
	int it = RTC->CRL & (RTC_IT_SEC | RTC_IT_ALR);
	RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_ALR);

	if(it & RTC_IT_SEC)
		OnRtcSecond();

	if(it & RTC_IT_ALR)
	{
		printf("Alarm activated\n");

		SetNextAlarm();

		OnRtcAlarm();
	}
}

void SetRtcCalibrationValue(uint8_t value)
{
	BKP_SetRTCCalibrationValue(value & BKP_RTCCR_CAL);
}

uint8_t GetRtcCalibrationValue()
{
	return BKP->RTCCR & BKP_RTCCR_CAL;
}

void WEAKREF OnRtcSecond()
{

}

void WEAKREF OnRtcAlarm()
{

}

