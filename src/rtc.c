/*
 * rtc.c
 *
 *  Created on: 21/09/2019
 */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "stm32f10x.h"
#include "rtc.h"

void InitRTCInterrupts()
{
	NVIC_InitTypeDef nvic;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* Enable the RTC Interrupt */
	nvic.NVIC_IRQChannel 					= RTC_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority 	= 1;
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

	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
}

void InitClock()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	PWR_BackupAccessCmd(ENABLE);

	InitRTCInterrupts();

	/* Check for a flag in the backup register which should already be set if the RTC
	 * has been programmed previously */
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		/* RTC has not been previously configured */
		printf("Initializing RTC and backup domain\n");

	    InitRTCOneTimeConfig();

	    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}

	/* Clear reset flags */
	RCC_ClearFlag();

	printf("Clock initialized\n");
}

void SetTime(struct tm* ptm)
{

}

void GetTime(struct tm* ptm)
{
	time_t tim = (time_t)RTC_GetCounter();

	struct tm* pltm = localtime(&tim);
	memcpy(ptm, pltm, sizeof(struct tm));
}

void RTC_IRQHandler()
{
	int it = RTC->CRH & (RTC_IT_SEC | RTC_IT_ALR);
	RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_ALR);

	if(it & RTC_IT_SEC)
		RTC_OnSecond();

	if(it & RTC_IT_ALR)
		RTC_OnAlarm();
}

void __attribute__((weak)) RTC_OnSecond()
{

}

void __attribute__((weak)) RTC_OnAlarm()
{

}

