/*
 * st7735.c
 *
 *  Created on: 22/09/2019
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "core_cmInstr.h"
#include "macros.h"
#include "system.h"
#include "graphics.h"
#include "graphics_impl.h"
#include "fonts.h"
#include "st7735.h"

enum DataCmd
{
	Cmd = 0,
	Data = 1
};

typedef struct
{
	uint8_t 	cmd;
	uint8_t		delay;
	uint8_t		datalen;
	uint8_t		data[16];
} InitStruct, *PInitStruct;

const InitStruct is[] =
{
		// Software reset
		{ 0x01, 150, 	0, 	{ 0 } },
		// Exit sleep mode
		{ 0x11,	150,	0, 	{ 0 } },
		// Frame rate
		{ 0xB1, 0, 		3, 	{ 0x01, 0x2C, 0x2D } },
		{ 0xB2, 0, 		3, 	{ 0x01, 0x2C, 0x2D } },
		// FRMCTR3 Frame Rate configuration -- partial mode
		{ 0xB3, 0, 		6, 	{ 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D } },
		// INVCTR Display inversion (no inversion)
		{ 0xB4,  0, 	1, 	{ 0x07 } },
		// PWCTR1 Power control -4.6V, Auto mode
		{ 0xC0,  0, 	3, 	{ 0xA2, 0x02, 0x84} },
		// PWCTR2 Power control VGH25 2.4C, VGSEL -10, VGH = 3 * AVDD
		{ 0xC1,  0, 	1, 	{ 0xC5} },
		// PWCTR3 Power control, opamp current smal, boost frequency
		{ 0xC2,  0, 	2, 	{ 0x0A, 0x00 } },
		// PWCTR4 Power control, BLK/2, opamp current small and medium low
		{ 0xC3,  0, 	2, 	{ 0x8A, 0x2A} },
		// PWRCTR5, VMCTR1 Power control
		{ 0xC4,  0, 	2, 	{ 0x8A, 0xEE} },
		{ 0xC5,  0, 	1, 	{ 0x0E } },
		// INVOFF - no inversion
		{ 0x20,  0,		0, { 0 } },
		// MADCTL Memory access directions. row address/col address, bottom to top refesh (10.1.27)
		{ 0x36,  0, 	1, 	{ 0xA0 } },
		// Color mode 16 bit (10.1.30
		{ 0x3A, 0, 		1, 	{ 0x05} },
		// GMCTRP1 Gamma correction
		{ 0xE0, 0, 		16, { 0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10 } },
		// GMCTRP2 Gamma Polarity correction
		{ 0xE1, 0, 		16,	{0x03, 0x1d, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10 } },
		// DISPON Display on
		{ 0x29, 100, 	0, 	{ 0 } },
		// NORON Normal on
		{ 0x13, 10, 	0, 	{ 0 } },
		// Finish
		{ 0x00, 0, 0, 	{ 0 } }
};

#define DC_PIN			GPIO_Pin_6
#define CS_PIN			GPIO_Pin_4
#define RS_PIN			GPIO_Pin_3
#define CL_PIN			GPIO_Pin_5
#define SD_PIN			GPIO_Pin_7
#define BL_PIN			GPIO_Pin_6

void WriteBuffer(enum DataCmd dc, void* pdata, uint16_t len)
{
	GPIO_WriteBit(GPIOA, DC_PIN, dc);
	GPIO_ResetBits(GPIOA, CS_PIN);

	//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	//	;

	uint8_t* pd = (uint8_t*)pdata;

	while(len--)
	{
		SPI_I2S_SendData(SPI1, *pd++);

		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
			;
	}

	GPIO_SetBits(GPIOA, CS_PIN);
}

void WriteShortRepeat(enum DataCmd dc, uint16_t data, uint16_t len)
{
	SPI_DataSizeConfig(SPI1, SPI_DataSize_16b);

	GPIO_WriteBit(GPIOA, DC_PIN, dc);
	GPIO_ResetBits(GPIOA, CS_PIN);

	//while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
	//	;

	while(len--)
	{
		SPI_I2S_SendData(SPI1, data);

		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET)
			;
	}

	GPIO_SetBits(GPIOA, CS_PIN);

	SPI_DataSizeConfig(SPI1, SPI_DataSize_8b);
}

static ALWAYS_INLINE void WriteByte(enum DataCmd dc, uint8_t data)
{
	WriteBuffer(dc, &data, 1);
}

static ALWAYS_INLINE void WriteShort(enum DataCmd dc, uint16_t data)
{
	uint16_t d = __REV16(data);
	WriteBuffer(dc, &d, 2);
}

void InitDisplay()
{
	SPI_InitTypeDef 			spi;
	GPIO_InitTypeDef 			gpio;
	TIM_TimeBaseInitTypeDef		timb;
	TIM_OCInitTypeDef			ocnt;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	GPIO_StructInit(&gpio);

	// Use pin for backlight PWM, TIM4 OC1
	gpio.GPIO_Mode				= GPIO_Mode_AF_PP;
	gpio.GPIO_Speed				= GPIO_Speed_50MHz;
	gpio.GPIO_Pin				= BL_PIN;
	GPIO_Init(GPIOB, &gpio);

	// Init timer for backlight PWM
	TIM_TimeBaseStructInit(&timb);
	TIM_OCStructInit(&ocnt);

	timb.TIM_Prescaler 			= (((SystemCoreClock /* / 2 */) / BACKLIGHT_PWM_HZ) - 1);
	timb.TIM_CounterMode 		= TIM_CounterMode_Up;
	timb.TIM_Period 			= 255;
	timb.TIM_ClockDivision 		= TIM_CKD_DIV1;
	timb.TIM_RepetitionCounter 	= 0;
	TIM_TimeBaseInit(TIM4, &timb);

	ocnt.TIM_OCMode 			= TIM_OCMode_PWM1;
	ocnt.TIM_Pulse 				= 100;
	ocnt.TIM_OutputState 		= TIM_OutputState_Enable;
	ocnt.TIM_OCPolarity 		= TIM_OCPolarity_High;
	ocnt.TIM_OCIdleState 		= TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM4, &ocnt);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);

	// Enable SPI1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);

	spi.SPI_Mode 				= SPI_Mode_Master;
	spi.SPI_BaudRatePrescaler 	= SPI_BaudRatePrescaler_2;
	spi.SPI_Direction 			= SPI_Direction_1Line_Tx;
	spi.SPI_CPOL 				= SPI_CPOL_Low;
	spi.SPI_CPHA 				= SPI_CPHA_1Edge;
	spi.SPI_CRCPolynomial		= 7;
	spi.SPI_DataSize 			= SPI_DataSize_8b;
	spi.SPI_FirstBit 			= SPI_FirstBit_MSB;
	spi.SPI_NSS 				= SPI_NSS_Soft;

	SPI_Init(SPI1, &spi);

	// NSS must be set to '1' due to NSS_Soft settings (otherwise it will be Multimaster mode).
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
	SPI_Cmd(SPI1, ENABLE);

	GPIO_StructInit(&gpio);

	// SCK = PA05, MOSI (SDA) = PA07
	gpio.GPIO_Mode				= GPIO_Mode_AF_PP;
	gpio.GPIO_Speed				= GPIO_Speed_50MHz;
	gpio.GPIO_Pin				= CL_PIN | SD_PIN;
	GPIO_Init(GPIOA, &gpio);

	// CS  = PA4 (software slave select)
	// RS  = PA3 (reset)
	// DC  = PA6 (data/command)
	// BL  = PA2 (backlight)
	gpio.GPIO_Mode				= GPIO_Mode_Out_PP;
	gpio.GPIO_Pin				= CS_PIN | DC_PIN | RS_PIN;
	GPIO_Init(GPIOA, &gpio);

	// Reset sequence
	GPIO_SetBits(GPIOA, CS_PIN);
	GPIO_SetBits(GPIOA, RS_PIN);
	sleep(10);

	GPIO_ResetBits(GPIOA, RS_PIN);
	sleep(10);

	GPIO_SetBits(GPIOA, RS_PIN);
	sleep(10);

	for(PInitStruct ps = (PInitStruct)is; ps->cmd != 0; ps++)
	{
		WriteByte(Cmd, ps->cmd);

		if(ps->datalen)
			WriteBuffer(Data, ps->data, ps->datalen);
		if(ps->delay)
			sleep(ps->delay);
	}

	printf("Display initialized\n");
}

void SetSleepMode(enum Mode sl)
{
	WriteByte(Cmd, (sl == On) ? 0x10 : 0x11);
	sleep(120);
}

void SetInversionMode(enum Mode in)
{
	WriteByte(Cmd, (in) ? 0x21 : 0x20);
}

void SetDisplayMode(enum Mode ds)
{
	WriteByte(Cmd, (ds == On) ? 0x29 : 0x28);
}

void SetIdleMode(enum Mode id)
{
	WriteByte(Cmd, (id == On) ? 0x39 : 0x38);
}

void SetBacklightLevel(uint8_t level)
{
	TIM4->CCR1 = level;
}

void AddressSet(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
	uint16_t buf[] = { __REV16(xs), __REV16(xe) };

	WriteByte(Cmd, 0x2a);	// Column address
	WriteBuffer(Data, buf, sizeof(buf));

	uint16_t buf2[] = { __REV16(ys), __REV16(ye) };

	WriteByte(Cmd, 0x2b);	// Row address
	WriteBuffer(Data, buf2, sizeof(buf2));

	WriteByte(Cmd, 0x2c);	// Memory write
}

void MeasureDisplay(uint16_t* w, uint16_t* h)
{
	*w	= SCREEN_WIDTH;
	*h	= SCREEN_HEIGHT;
}

void ClearScreen()
{
	AddressSet(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	WriteShortRepeat(Data, 0x00, SCREEN_WIDTH * SCREEN_HEIGHT);
}

void SetPixelImpl(PDrawOp pd)
{
    AddressSet(pd->l, pd->t, pd->l, pd->t);
    WriteShort(Data, pd->fg);
}

void DrawRectImpl(PDrawOp pd)
{
	AddressSet(pd->l, pd->t, pd->l + pd->w - 1, pd->t + pd->h - 1);
	WriteShortRepeat(Data, pd->bg, (pd->w * pd->h));
}

Colour buf[32];
int bufind;

void ALWAYS_INLINE FlushBuf()
{
	if(bufind > 0)
	{
		WriteBuffer(Data, buf, bufind * 2);
		bufind = 0;
	}
}

void BlitLine1BPP(uint8_t* psrc, uint16_t ofs, uint16_t cnt, uint32_t colours)
{
	uint8_t bcnt;
	uint8_t src;
	uint16_t f		= __REV16(colours >> 16);
	uint16_t b 		= __REV16(colours & 0xFFFF);

	div_t d			= div(ofs, 8);
	bcnt			= 7 - d.rem;
	psrc 			+= d.quot;
	src				= *psrc++;

	while(cnt--)
	{
		buf[bufind++]	= (src & 0x80)	? f : b;

		if(bufind >= (sizeof(buf) / sizeof(Colour)))
			FlushBuf();

		src <<= 1;
		if(bcnt-- == 0)
		{
			src = *psrc++;
			bcnt = 7;
		}
	}
}

void BlitLineEx1BPP(uint8_t* psrc, uint32_t ofscnt, uint32_t lt, brushcallback_ptr pbr)
{
	uint8_t	bcnt;
	uint8_t src;
	uint16_t cnt	= ofscnt & 0xFFFF;
	uint16_t l		= lt >> 16;
	uint16_t t		= lt & 0xFFFF;

	div_t d			= div(ofscnt >> 16, 8);
	bcnt			= 7 - d.rem;
	psrc 			+= d.quot;
	src				= *psrc++;

	while(cnt--)
	{
		Colours clrs	= pbr(l++, t);

		buf[bufind++]	= (src & 0x80)	? __REV16(clrs.fore) : __REV16(clrs.back);

		if(bufind >= (sizeof(buf) / sizeof(Colour)))
			FlushBuf();

		src <<= 1;
		if(bcnt-- == 0)
		{
			src = *psrc++;
			bcnt = 7;
		}
	}
}

void BlitLine16BPP(uint16_t* psrc, uint16_t ofs, uint16_t cnt)
{
	psrc += ofs;
	while(cnt--)
	{
		buf[bufind++]	= __REV16(*psrc++);

		if(bufind >= (sizeof(buf) / sizeof(Colour)))
			FlushBuf();
	}
}

void DrawTextImpl(PDrawOp pd, const char* psz)
{
	AddressSet(pd->l, pd->t, (pd->l + pd->w) - 1, (pd->t + pd->h) - 1);

	PFont pf 			= pd->ft;
	uint32_t clr		= (uint32_t)pd->fg << 16 | pd->bg;
	uint16_t w			= pd->w;

	for(uint16_t ln = pd->toff; ln < pd->h; ln++)
	{
		uint16_t loff 	= pd->loff;
		uint16_t cnt	= pf->width - loff;
		for(const char* p = psz; *p; p++)
		{
			uint8_t* pfd	= pf->data + ((*p-pf->offset) * (pf->cellwidth * pf->height)) + (pf->cellwidth * ln);
			BlitLine1BPP(pfd, loff, cnt, clr);

			loff			= 0;
			cnt				= (w -= cnt) > pf->width ? pf->width : w;
		}

		FlushBuf();
	}
}

void DrawTextExImpl(PDrawOp pd, const char* psz)
{
	AddressSet(pd->l, pd->t, (pd->l + pd->w) - 1, (pd->t + pd->h) - 1);

	PFont pf 			= pd->ft;
	uint16_t w			= pd->w;

	for(uint16_t ln = pd->toff; ln < pd->h; ln++)
	{
		uint16_t l		= pd->loff;
		uint16_t loff 	= pd->loff;
		uint16_t cnt	= pf->width - loff;
		for(const char* p = psz; *p; p++)
		{
			uint8_t* pfd	= pf->data + ((*p-pf->offset) * (pf->cellwidth * pf->height)) + (pf->cellwidth * ln);
			BlitLineEx1BPP(pfd, (loff << 16) | cnt, (l << 16) | ln, pd->pbr);

			l				+= cnt;
			loff			= 0;
			cnt				= (w -= cnt) > pf->width ? pf->width : w;
		}

		FlushBuf();
	}
}

void DrawGradientVertical(uint16_t l, uint16_t t, uint16_t w, uint16_t h, uint16_t offset)
{
	AddressSet(l, t, (l + w) - 1, (t + h) - 1);

	double r = h + offset;
	for(double i=offset; i<r; i++)
		WriteShortRepeat(Data, Gradient(i / r), w);
}

void DrawBitmapImpl(PDrawOp pd, uint16_t l, uint16_t t, PBitmap bm)
{
	uint32_t clr		= (uint32_t)pd->fg << 16 | pd->bg;

	AddressSet(l, t, (l + bm->width) - 1, (t + bm->height) - 1);

	uint32_t ri = (bm->width >> 3) + ((bm->width & 0x07) ? 1 : 0);

	switch(bm->colour)
	{
	case Colour1Bpp:
		for(uint16_t r = 0; r < bm->height; r++)
			BlitLine1BPP(bm->data + (r * ri), 0, bm->width, clr);
		break;
	case Colour16Bpp:
		BlitLine16BPP(bm->data, 0, (bm->width * bm->height));
		break;
	default:
		break;
	}

	FlushBuf();
}

void DrawBitmapExImpl(PDrawOp pd, uint16_t l, uint16_t t, PBitmap bm)
{
	AddressSet(l, t, (l + bm->width) - 1, (t + bm->height) - 1);

	uint32_t ri = (bm->width >> 3) + ((bm->width & 0x07) ? 1 : 0);

	for(uint16_t r = 0; r < bm->height; r++)
		BlitLineEx1BPP(bm->data + (r * ri), bm->width, r, pd->pbr);

	FlushBuf();
}
