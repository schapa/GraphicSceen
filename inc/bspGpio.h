/*
 * bspGpio.h
 *
 *  Created on: Jun 15, 2017
 *      Author: shapa
 */

#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>

typedef struct GpioCfg_t GpioCfg_t;

typedef enum {
	GPIO_KEY_WAKE_USER, // pa0

	GPIO_LED_GREEN, //pg13
	GPIO_LED_RED, //pg14

	GPIO_MEMS_INT_1, // pa1
	GPIO_MEMS_INT_2, // pa2
	GPIO_MEMS_CS, // pc1
	GPIO_SPI5_SCK, // pf7
	GPIO_SPI5_MISO, // pf8
	GPIO_SPI5_MOSI, // pf9

	GPIO_LCD_R2, //pc10
	GPIO_LCD_R3, //pb0
	GPIO_LCD_R4, //pa11
	GPIO_LCD_R5, //pa12
	GPIO_LCD_R6, //pb1
	GPIO_LCD_R7, //pg6

	GPIO_LCD_G2, //pa6
	GPIO_LCD_G3, //pg10
	GPIO_LCD_G4, //pb10
	GPIO_LCD_G5, //pb11
	GPIO_LCD_G6, //pc7
	GPIO_LCD_G7, //pd3

	GPIO_LCD_B2, //pd6
	GPIO_LCD_B3, //pg11
	GPIO_LCD_B4, //pg12
	GPIO_LCD_B5, //pa3
	GPIO_LCD_B6, //pb8
	GPIO_LCD_B7, //pb9

	GPIO_LCD_VSYNC, //pa4
	GPIO_LCD_HSYNC, //pc6
	GPIO_LCD_CSX, //pc2
	GPIO_LCD_TE, //pd11
	GPIO_LCD_RDX, //pd12
	GPIO_LCD_WRX, //pd13
	GPIO_LCD_ENABLE, //pf10
	GPIO_LCD_DOTCLK, //pg7

	GPIO_LCD_IM0, //pd2 interface SELECT
	GPIO_LCD_IM1, //pd4
	GPIO_LCD_IM2, //pd5
	GPIO_LCD_IM3, //pd7

	GPIO_I2C3_SCL, // pa8
	GPIO_I2C3_SCA, // pc9
	GPIO_TOUCH_INT, //pa15

	GPIO_USART1_TX, //pa9
	GPIO_USART1_RX, //pa10

	GPIO_SDRAM_DATA_0, //pd14
	GPIO_SDRAM_DATA_1, //pd15
	GPIO_SDRAM_DATA_2, //pd0
	GPIO_SDRAM_DATA_3, //pd1
	GPIO_SDRAM_DATA_4, //pe7
	GPIO_SDRAM_DATA_5, //pe8
	GPIO_SDRAM_DATA_6, //pe9
	GPIO_SDRAM_DATA_7, //pe10
	GPIO_SDRAM_DATA_8, //pe11
	GPIO_SDRAM_DATA_9, //pe12
	GPIO_SDRAM_DATA_10, //pe13
	GPIO_SDRAM_DATA_11, //pe14
	GPIO_SDRAM_DATA_12, //pe15
	GPIO_SDRAM_DATA_13, //pd8
	GPIO_SDRAM_DATA_14, //pd9
	GPIO_SDRAM_DATA_15, //pd10
	GPIO_SDRAM_ADR_0, //pf0
	GPIO_SDRAM_ADR_1, //pf1
	GPIO_SDRAM_ADR_2, //pf2
	GPIO_SDRAM_ADR_3, //pf3
	GPIO_SDRAM_ADR_4, //pg4
	GPIO_SDRAM_ADR_5, //pf5
	GPIO_SDRAM_ADR_6, //pf12
	GPIO_SDRAM_ADR_7, //pf13
	GPIO_SDRAM_ADR_8, //pf14
	GPIO_SDRAM_ADR_9, //pf15
	GPIO_SDRAM_ADR_10, //pg0
	GPIO_SDRAM_ADR_11, //pg1
	GPIO_SDRAM_CKE1, //pb5
	GPIO_SDRAM_NE1, //pb6
	GPIO_SDRAM_NWE, //pc0
	GPIO_SDRAM_NBL0, //pe0
	GPIO_SDRAM_NBL1, //pe1
	GPIO_SDRAM_NRAS, //pf11
	GPIO_SDRAM_BA0, //pg4
	GPIO_SDRAM_BA1, //pg5
	GPIO_SDRAM_CLK, //pg8
	GPIO_SDRAM_NCAS, //pg15

	GPIO_USB_OTG_FS_ID, //pb12
	GPIO_USB_VBUS_FS, //pb13
	GPIO_USB_OTG_FS_DM, //pb14
	GPIO_USB_OTG_FS_DP, //pb15
	GPIO_USB_OTG_FS_PCO, //pc4
	GPIO_USB_OTG_FS_OC, //pc5

	GPIO_LAST
} Gpio_e;

enum {
	GPIO_USER_0 = GPIO_LAST, //pa5    pwm
	GPIO_USER_1, //pa7
	GPIO_USER_2, //pb2 boot 1
	GPIO_USER_3, //pb3
	GPIO_USER_4, //pb4
	GPIO_USER_5, //pb7
	GPIO_USER_6, //pc3
	GPIO_USER_7, //pc8
	GPIO_USER_8, //pc11
	GPIO_USER_9, //pc12
	GPIO_USER_10, //pc13
	GPIO_USER_11, //pe2
	GPIO_USER_12, //pe3
	GPIO_USER_13, //pe4
	GPIO_USER_14, //pe5
	GPIO_USER_15, //pe6
	GPIO_USER_16, //pf6
	GPIO_USER_17, //pg2
	GPIO_USER_18, //pg3
	GPIO_USER_19, //pg9
};


void BSP_Gpio_Init(void);
void BSP_Gpio_Init_Pin(const Gpio_e pin);
const _Bool BSP_Gpio_ReadPin(const Gpio_e pin);
void BSP_Gpio_SetPin(const Gpio_e pin, const _Bool val);
const GpioCfg_t *const BSP_Gpio_CfgGet(const Gpio_e pin);

#ifdef __cplusplus
 }
#endif
