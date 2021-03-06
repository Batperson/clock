/*
 * st7735.h
 *
 *  Created on: 22/09/2019
 */

#ifndef ST7735_H_
#define ST7735_H_

#define SCREEN_WIDTH		160
#define SCREEN_HEIGHT		128
#define BACKLIGHT_PWM_HZ	1000 // 250

enum Mode
{
	Off	= 0x00,
	On	= 0x01
};

void SetSleepMode(enum Mode sl);
void SetInversionMode(enum Mode in);
void SetDisplayMode(enum Mode ds);
void SetIdleMode(enum Mode id);
void SetBacklightLevel(uint8_t level);


#endif /* ST7735_H_ */
