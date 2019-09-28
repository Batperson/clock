/*
 * swo.c
 *
 */

#include "stm32f10x.h"

int _write(int fd, char *str, int len)
{
	for(int i=0; i<len; i++)
		ITM_SendChar((*str++));

	return len;
}
