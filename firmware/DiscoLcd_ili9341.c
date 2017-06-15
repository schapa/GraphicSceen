/*
 * lcd.c
 *
 *  Created on: Aug 21, 2016
 *      Author: shapa
 */

/**
  * @brief  LCD Registers
  */
//#define ILI9341_SLEEP_OUT            0x11   /* Sleep out register */
//#define ILI9341_SET_GAMMA                0x26   /* Gamma register */
//#define ILI9341_DISPLAY_OFF          0x28   /* Display off register */
//#define ILI9341_DISPLAY_ON           0x29   /* Display on register */
//#define ILI9341_SET_COLUMN_ADDRESS          0x2A   /* Colomn address register */
//#define ILI9341_SET_PAGE_ADDRESS            0x2B   /* Page address register */
//#define ILI9341_MEMORY_WRITE                 0x2C   /* GRAM register */
//#define ILI9341_MEMORY_ACCESS_CTRL                  0x36   /* Memory Access Control register*/
//#define ILI9341_SET_PIXEL_FORMAT         0x3A   /* Pixel Format register */
//#define ILI9341_SET_BRIGHTNESS                  0x51   /* Write Brightness Display register */
//#define ILI9341_SET_WRT_CTRL                  0x53   /* Write Control Display register*/
//#define ILI9341_SET_RGB_SIG_CTRL        0xB0   /* RGB Interface Signal Control */
//#define ILI9341_SET_FRAME_CTRL_NORMAL                  0xB1   /* Frame Rate Control register */
//#define ILI9341_SET_BLANK_PORCH                  0xB5   /* Blanking Porch Control register*/
//#define ILI9341_SET_CTRL                  0xB6   /* Display Function Control register*/
//#define ILI9341_SET_POWER_CTRL_1               0xC0   /* Power Control 1 register */
//#define ILI9341_SET_POWER_CTRL_1               0xC1   /* Power Control 2 register */
//#define ILI9341_SET_VCOM_CTRL_1                0xC5   /* VCOM Control 1 register */
//#define ILI9341_SET_VCOM_CTRL_2                0xC7   /* VCOM Control 2 register */
#define LCD_POWERA               0xCB   /* Power control A register */
#define LCD_POWERB               0xCF   /* Power control B register */
#define LCD_PGAMMA               0xE0   /* Positive Gamma Correction register*/
#define LCD_NGAMMA               0xE1   /* Negative Gamma Correction register*/
#define LCD_DTCA                 0xE8   /* Driver timing control A */
#define LCD_DTCB                 0xEA   /* Driver timing control B */
#define LCD_POWER_SEQ            0xED   /* Power on sequence register */
#define LCD_3GAMMA_EN            0xF2   /* 3 Gamma enable register */
//#define ILI9341_IFACE_CTRL            0xF6   /* Interface control register */
#define LCD_PRC                  0xF7   /* Pump ratio control register */


#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "bsp.h"
#include "system.h"
#include "ili9341.h"

static void chipSelect(_Bool mode);
static void setDataMode(_Bool mode);

static void sendLcdCmd(uint8_t val);
static void sendLcd(uint8_t val);

static void initILI9341(void);

static void initLayer(uint8_t *buff);

static LTDC_HandleTypeDef s_ltdc;


static const uint16_t s_lcdWidth = 240;
static const uint16_t s_lcdHeight = 320;

void DiscoLCDInit(uint8_t *buff) {

	LTDC_InitTypeDef iface = {
			LTDC_HSPOLARITY_AL,
			LTDC_VSPOLARITY_AL,
			LTDC_DEPOLARITY_AL,
			LTDC_PCPOLARITY_IPC,
			.HorizontalSync = 9,
			.VerticalSync = 1,
			.AccumulatedHBP = 29,
			.AccumulatedVBP = 3,
//			.AccumulatedActiveW = s_lcdWidth + 29,
//			.AccumulatedActiveH = s_lcdHeight + 3,
//			.TotalWidth = s_lcdWidth + 39,
//			.TotalHeigh = s_lcdHeight + 7,
			.Backcolor.Red = 0,
			.Backcolor.Green = 0,
			.Backcolor.Blue = 0,
	};

	iface.AccumulatedActiveW = s_lcdWidth + iface.AccumulatedHBP;
	iface.AccumulatedActiveH = s_lcdHeight + iface.AccumulatedVBP;
	iface.TotalWidth = s_lcdWidth + iface.HorizontalSync + iface.VerticalSync + iface.AccumulatedHBP;
	iface.TotalHeigh = s_lcdHeight + 7;

	s_ltdc.Init = iface;
	s_ltdc.Instance = LTDC;

	chipSelect(false);
	setDataMode(true);

	initILI9341();

	HAL_LTDC_Init(&s_ltdc);
	HAL_LTDC_EnableDither(&s_ltdc);
	initLayer(buff);
}

void DiscoLCD_setState(_Bool state) {
	if (state) {
		HAL_LTDC_SetAlpha(&s_ltdc, 255, 0);
	}
	else {
		HAL_LTDC_SetAlpha(&s_ltdc, 0, 0);
	}
}

static void chipSelect(_Bool mode) {
	GPIO_PinState val = mode ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, val);
}

static void setDataMode(_Bool mode) {
	GPIO_PinState val = mode ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, val);
}

static void sendLcdCmd(uint8_t val) {
	setDataMode(false);
	chipSelect(true);
	HAL_SPI_Transmit(BSP_GetHandleSpi_5(), &val, 1, 0xFFFF);
	chipSelect(false);
}


static void sendLcd(uint8_t val)  {
	setDataMode(true);
	chipSelect(true);
	HAL_SPI_Transmit(BSP_GetHandleSpi_5(), &val, 1, 0xFFFF);
	chipSelect(false);
}

static void initLayer(uint8_t *buff) {
	const uint16_t width = s_lcdWidth;
	const uint16_t height = 64;

	uintptr_t fb = (uintptr_t)buff;

	LTDC_LayerCfgTypeDef layer = {
			.WindowX0 = 0,
			.WindowX1 = width - 1,
			.WindowY0 = 0,
			.WindowY1 = height - 1,
			.PixelFormat = LTDC_PIXEL_FORMAT_RGB565,
			.Alpha = 255,
			.Alpha0 = 255,
			.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA,
			.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA,
			.FBStartAdress = fb,
			.ImageWidth = width,
			.ImageHeight = height,
			.Backcolor = s_ltdc.Init.Backcolor
	};

	HAL_LTDC_ConfigLayer(&s_ltdc, &layer, 0);
}

static void initILI9341(void) {
	sendLcdCmd(0xCA);
	sendLcd(0xC3);
	sendLcd(0x08);
	sendLcd(0x50);
	sendLcdCmd(LCD_POWERB);
	sendLcd(0x00);
	sendLcd(0xC1);
	sendLcd(0x30);
	sendLcdCmd(LCD_POWER_SEQ);
	sendLcd(0x64);
	sendLcd(0x03);
	sendLcd(0x12);
	sendLcd(0x81);
	sendLcdCmd(LCD_DTCA);
	sendLcd(0x85);
	sendLcd(0x00);
	sendLcd(0x78);
	sendLcdCmd(LCD_POWERA);
	sendLcd(0x39);
	sendLcd(0x2C);
	sendLcd(0x00);
	sendLcd(0x34);
	sendLcd(0x02);
	sendLcdCmd(LCD_PRC);
	sendLcd(0x20);
	sendLcdCmd(LCD_DTCB);
	sendLcd(0x00);
	sendLcd(0x00);
	sendLcdCmd(ILI9341_SET_FRAME_CTRL_NORMAL);
	sendLcd(0x00);
	sendLcd(0x1B);
	sendLcdCmd(ILI9341_SET_CTRL);
	sendLcd(0x0A);
	sendLcd(0xA2);
	sendLcdCmd(ILI9341_SET_POWER_CTRL_1);
	sendLcd(0x10);
	sendLcdCmd(ILI9341_SET_POWER_CTRL_1);
	sendLcd(0x10);
	sendLcdCmd(ILI9341_SET_VCOM_CTRL_1);
	sendLcd(0x45);
	sendLcd(0x15);
	sendLcdCmd(ILI9341_SET_VCOM_CTRL_2);
	sendLcd(0x90);
	sendLcdCmd(ILI9341_MEMORY_ACCESS_CTRL);
	sendLcd(0xC8);
	sendLcdCmd(LCD_3GAMMA_EN);
	sendLcd(0x00);
	sendLcdCmd(ILI9341_SET_RGB_SIG_CTRL);
	sendLcd(0xC2);
	sendLcdCmd(ILI9341_SET_CTRL);
	sendLcd(0x0A);
	sendLcd(0xA7);
	sendLcd(0x27);
	sendLcd(0x04);

	/* colomn address set */
	sendLcdCmd(ILI9341_SET_COLUMN_ADDRESS);
	sendLcd(0x00);
	sendLcd(0x00);
	sendLcd(0x00);
	sendLcd(0xEF);
	/* Page Address Set */
	sendLcdCmd(ILI9341_SET_PAGE_ADDRESS);
	sendLcd(0x00);
	sendLcd(0x00);
	sendLcd(0x01);
	sendLcd(0x3F);
	sendLcdCmd(ILI9341_IFACE_CTRL);
	sendLcd(0x01);
	sendLcd(0x00);
	sendLcd(0x06);

	sendLcdCmd(ILI9341_MEMORY_WRITE);
	System_delayMsDummy(200);

	sendLcdCmd(ILI9341_SET_GAMMA);
	sendLcd(0x01);

	sendLcdCmd(LCD_PGAMMA);
	sendLcd(0x0F);
	sendLcd(0x29);
	sendLcd(0x24);
	sendLcd(0x0C);
	sendLcd(0x0E);
	sendLcd(0x09);
	sendLcd(0x4E);
	sendLcd(0x78);
	sendLcd(0x3C);
	sendLcd(0x09);
	sendLcd(0x13);
	sendLcd(0x05);
	sendLcd(0x17);
	sendLcd(0x11);
	sendLcd(0x00);
	sendLcdCmd(LCD_NGAMMA);
	sendLcd(0x00);
	sendLcd(0x16);
	sendLcd(0x1B);
	sendLcd(0x04);
	sendLcd(0x11);
	sendLcd(0x07);
	sendLcd(0x31);
	sendLcd(0x33);
	sendLcd(0x42);
	sendLcd(0x05);
	sendLcd(0x0C);
	sendLcd(0x0A);
	sendLcd(0x28);
	sendLcd(0x2F);
	sendLcd(0x0F);

	sendLcdCmd(ILI9341_SLEEP_OUT);
	System_delayMsDummy(200);
	sendLcdCmd(ILI9341_DISPLAY_ON);
	/* GRAM start writing */
	sendLcdCmd(ILI9341_MEMORY_WRITE);
 }
