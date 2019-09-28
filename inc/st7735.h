/*
 * st7735.h
 *
 *  Created on: 22/09/2019
 */

#ifndef ST7735_H_
#define ST7735_H_

#define SCREEN_WIDTH	162
#define SCREEN_HEIGHT	106

enum Mode
{
	Off	= 0x00,
	On	= 0x01
};

void SetBacklight(enum Mode bl);
void SetSleepMode(enum Mode sl);
void SetInversionMode(enum Mode in);
void SetDisplayMode(enum Mode ds);
void SetIdleMode(enum Mode id);


#endif /* ST7735_H_ */
