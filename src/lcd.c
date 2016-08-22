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
#include "bsp.h"
#include "system.h"
#include "ili9341.h"

static void initGPIO_Ctrl(void);
static void initSpi(void);
static void initGPIO_LcdRgb(void);

static void chipSelect(_Bool mode);
static void setDataMode(_Bool mode);

static void sendLcdCmd(uint8_t val);
static void sendLcd(uint8_t val);

static void initILI9341(void);

static SPI_HandleTypeDef s_spi;

void DiscoLCDInit(void) {

//	LTDC_InitTypeDef iface = {
//			LTDC_HSPOLARITY_AL,
//			LTDC_VSPOLARITY_AL,
//			LTDC_DEPOLARITY_AL,
//			LTDC_PCPOLARITY_IPC,
//			9,
//			1,
//			29,
//			3,
//			269,
//			323,
//			279,
//			327,
//	};

	initGPIO_Ctrl();

	initSpi();

	initILI9341();

	initGPIO_LcdRgb();

	setDataMode(true);
	chipSelect(true);
//	for (uint32_t i = 0; i < 0xFFFFFF; i++) {
//		HAL_SPI_Transmit(&s_spi, &i, 1, 0xFFFF);
//	}
	uint32_t i = 0;
	while(1) {
		HAL_SPI_Transmit(&s_spi, &i, 1, 0xFFFF);
		i++;
	}
	chipSelect(false);

//	  LCD_AF_GPIOConfig();


//  LTDC_Init(&iface);
}


static void initGPIO_Ctrl(void) {

	GPIO_InitTypeDef iface = {
			GPIO_PIN_2,
			GPIO_MODE_OUTPUT_PP,
			GPIO_NOPULL,
			GPIO_SPEED_FREQ_HIGH,
			0
	};

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	HAL_GPIO_Init(GPIOC, &iface);

	iface.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOD, &iface);

	chipSelect(false);
	setDataMode(true);
}

static void initSpi(void) {

	SPI_InitTypeDef iface = {
		SPI_MODE_MASTER,
		SPI_DIRECTION_2LINES,
		SPI_DATASIZE_8BIT,
		SPI_POLARITY_LOW,
		SPI_PHASE_1EDGE,
		SPI_NSS_SOFT,
		SPI_BAUDRATEPRESCALER_16,//			SPI_BAUDRATEPRESCALER_256,
		SPI_FIRSTBIT_MSB,
		SPI_TIMODE_DISABLE,
		SPI_CRCCALCULATION_DISABLE,
		0xABCD
	};
	s_spi.Instance = SPI5;
	s_spi.Init = iface;
	HAL_SPI_Init(&s_spi);
}

static void initGPIO_LcdRgb(void) {
/* GPIOs Configuration */
/*
 +------------------------+-----------------------+----------------------------+
 +                       LCD pins assignment                                   +
 +------------------------+-----------------------+----------------------------+
 |  LCD_TFT R2 <-> PC.10  |  LCD_TFT G2 <-> PA.06 |  LCD_TFT B2 <-> PD.06      |
 |  LCD_TFT R3 <-> PB.00  |  LCD_TFT G3 <-> PG.10 |  LCD_TFT B3 <-> PG.11      |
 |  LCD_TFT R4 <-> PA.11  |  LCD_TFT G4 <-> PB.10 |  LCD_TFT B4 <-> PG.12      |
 |  LCD_TFT R5 <-> PA.12  |  LCD_TFT G5 <-> PB.11 |  LCD_TFT B5 <-> PA.03      |
 |  LCD_TFT R6 <-> PB.01  |  LCD_TFT G6 <-> PC.07 |  LCD_TFT B6 <-> PB.08      |
 |  LCD_TFT R7 <-> PG.06  |  LCD_TFT G7 <-> PD.03 |  LCD_TFT B7 <-> PB.09      |
 -------------------------------------------------------------------------------
		  |  LCD_TFT HSYNC <-> PC.06  | LCDTFT VSYNC <->  PA.04 |
		  |  LCD_TFT CLK   <-> PG.07  | LCD_TFT DE   <->  PF.10 |
		   -----------------------------------------------------

*/
	GPIO_InitTypeDef iface = {
			GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_11 | GPIO_PIN_12,
			GPIO_MODE_OUTPUT_PP,
			GPIO_NOPULL,
			GPIO_SPEED_FREQ_HIGH,
			GPIO_AF14_LTDC
	};

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	HAL_GPIO_Init(GPIOA, &iface);

	iface.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
	HAL_GPIO_Init(GPIOB, &iface);
	iface.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	iface.Alternate = GPIO_AF9_LTDC;
	HAL_GPIO_Init(GPIOB, &iface);

	iface.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10;
	iface.Alternate = GPIO_AF14_LTDC;
	HAL_GPIO_Init(GPIOC, &iface);

	iface.Pin = GPIO_PIN_3 | GPIO_PIN_6;
	HAL_GPIO_Init(GPIOD, &iface);

	iface.Pin = GPIO_PIN_10 | GPIO_PIN_6;
	HAL_GPIO_Init(GPIOF, &iface);

	iface.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11;
	HAL_GPIO_Init(GPIOG, &iface);

	iface.Pin = GPIO_PIN_10 | GPIO_PIN_12;
	iface.Alternate = GPIO_AF9_LTDC;
	HAL_GPIO_Init(GPIOG, &iface);
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
	HAL_SPI_Transmit(&s_spi, &val, 1, 0xFFFF);
	chipSelect(false);
}


static void sendLcd(uint8_t val)  {
	setDataMode(true);
	chipSelect(true);
	HAL_SPI_Transmit(&s_spi, &val, 1, 0xFFFF);
	chipSelect(false);
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
//	sendLcdCmd(ILI9341_IFACE_CTRL);
//	sendLcd(0x01);
//	sendLcd(0x00);
//	sendLcd(0x06);

	sendLcdCmd(ILI9341_MEMORY_WRITE);
	System_delayMsDummy(10);

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
	System_delayMsDummy(10);
	sendLcdCmd(ILI9341_DISPLAY_ON);
	/* GRAM start writing */
	sendLcdCmd(ILI9341_MEMORY_WRITE);
 }
