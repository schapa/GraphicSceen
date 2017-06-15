/*
 * bspGpio.c
 *
 *  Created on: Jun 15, 2017
 *      Author: shapa
 */

#include "stm32f4xx_hal.h"
#include "bspGpio.h"
#include <assert.h>

struct GpioCfg_t{
	GPIO_TypeDef *const port;
	const GPIO_InitTypeDef config;
};

static const GpioCfg_t s_defCon[] = {
	[GPIO_KEY_WAKE_USER] =
		{ GPIOA, { GPIO_PIN_0, GPIO_MODE_IT_RISING_FALLING, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW, 0 } },

	[GPIO_LED_GREEN] =
		{ GPIOG, { GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0 } },
	[GPIO_LED_RED] =
		{ GPIOG, { GPIO_PIN_14, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0 } },

	[GPIO_MEMS_INT_1] =
		{ GPIOA, { GPIO_PIN_1, GPIO_MODE_IT_RISING_FALLING, GPIO_PULLDOWN, GPIO_SPEED_FREQ_MEDIUM, 0 } },
	[GPIO_MEMS_INT_2] =
		{ GPIOA, { GPIO_PIN_2, GPIO_MODE_IT_RISING_FALLING, GPIO_PULLDOWN, GPIO_SPEED_FREQ_MEDIUM, 0 } },
	[GPIO_MEMS_CS] =
		{ GPIOC, { GPIO_PIN_1, GPIO_MODE_IT_RISING_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM, GPIO_AF5_SPI5 /*6*/ } },
	[GPIO_SPI5_SCK] =
		{ GPIOF, { GPIO_PIN_7, GPIO_MODE_IT_RISING_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM, GPIO_AF5_SPI5 /*6*/ } },
	[GPIO_SPI5_MISO] =
		{ GPIOF, { GPIO_PIN_8, GPIO_MODE_IT_RISING_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM, GPIO_AF5_SPI5 /*6*/ } },
	[GPIO_SPI5_MOSI] =
		{ GPIOF, { GPIO_PIN_9, GPIO_MODE_IT_RISING_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM, GPIO_AF5_SPI5 /*6*/ } },

	[GPIO_LCD_R2] =
		{ GPIOC, { GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_R3] =
		{ GPIOB, { GPIO_PIN_0, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_R4] =
		{ GPIOA, { GPIO_PIN_11, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_R5] =
		{ GPIOA, { GPIO_PIN_12, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_R6] =
		{ GPIOB, { GPIO_PIN_1, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_R7] =
		{ GPIOG, { GPIO_PIN_6, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_G2] =
		{ GPIOA, { GPIO_PIN_6, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_G3] =
		{ GPIOG, { GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_G4] =
		{ GPIOB, { GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_G5] =
		{ GPIOB, { GPIO_PIN_11, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_G6] =
		{ GPIOC, { GPIO_PIN_7, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_G7] =
		{ GPIOD, { GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_B2] =
		{ GPIOD, { GPIO_PIN_6, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_B3] =
		{ GPIOG, { GPIO_PIN_11, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_B4] =
		{ GPIOG, { GPIO_PIN_12, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_B5] =
		{ GPIOA, { GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_B6] =
		{ GPIOB, { GPIO_PIN_8, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_B7] =
		{ GPIOB, { GPIO_PIN_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_VSYNC] =
		{ GPIOA, { GPIO_PIN_4, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_HSYNC] =
		{ GPIOC, { GPIO_PIN_6, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_CSX] =
		{ GPIOC, { GPIO_PIN_2, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_TE] =
		{ GPIOD, { GPIO_PIN_11, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_RDX] =
		{ GPIOD, { GPIO_PIN_12, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_WRX] =
		{ GPIOD, { GPIO_PIN_13, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
//	[GPIO_LCD_SCL] =
//		{ GPIOF, { GPIO_PIN_7, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
//	[GPIO_LCD_SDA] =
//		{ GPIOF, { GPIO_PIN_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_ENABLE] =
		{ GPIOF, { GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },
	[GPIO_LCD_DOTCLK] =
		{ GPIOG, { GPIO_PIN_7, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF14_LTDC } },

	[GPIO_I2C3_SCL] =
		{ GPIOA, { GPIO_PIN_8, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF4_I2C3 /*GPIO_AF9_I2C3*/ } },
	[GPIO_I2C3_SCA] =
		{ GPIOC, { GPIO_PIN_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF4_I2C3 /*GPIO_AF9_I2C3*/ } },


	[GPIO_USART1_TX] =
		{ GPIOA, { GPIO_PIN_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART1 } },
	[GPIO_USART1_RX] =
		{ GPIOA, { GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF7_USART1 } },

	[GPIO_TOUCH_INT] =
		{ GPIOA, { GPIO_PIN_15, GPIO_MODE_IT_RISING_FALLING, GPIO_PULLDOWN, GPIO_SPEED_FREQ_MEDIUM, 0 } },

	[GPIO_SDRAM_DATA_0] =
		{ GPIOD, { GPIO_PIN_14, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_1] =
		{ GPIOD, { GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_2] =
		{ GPIOD, { GPIO_PIN_0, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_3] =
		{ GPIOD, { GPIO_PIN_1, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_4] =
		{ GPIOE, { GPIO_PIN_7, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_5] =
		{ GPIOE, { GPIO_PIN_8, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_6] =
		{ GPIOE, { GPIO_PIN_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_7] =
		{ GPIOE, { GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_8] =
		{ GPIOE, { GPIO_PIN_11, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_9] =
		{ GPIOE, { GPIO_PIN_12, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_10] =
		{ GPIOE, { GPIO_PIN_13, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_11] =
		{ GPIOE, { GPIO_PIN_14, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_12] =
		{ GPIOE, { GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_13] =
		{ GPIOD, { GPIO_PIN_8, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_14] =
		{ GPIOD, { GPIO_PIN_9, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_DATA_15] =
		{ GPIOD, { GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_ADR_0] =
		{ GPIOF, { GPIO_PIN_0, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_ADR_1] =
		{ GPIOF, { GPIO_PIN_1, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_ADR_2] =
		{ GPIOF, { GPIO_PIN_2, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_ADR_3] =
		{ GPIOF, { GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_ADR_4] =
		{ GPIOF, { GPIO_PIN_4, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_ADR_5] =
		{ GPIOF, { GPIO_PIN_5, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_ADR_6] =
		{ GPIOF, { GPIO_PIN_12, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_ADR_7] =
		{ GPIOF, { GPIO_PIN_13, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_ADR_8] =
		{ GPIOF, { GPIO_PIN_14, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_ADR_9] =
		{ GPIOF, { GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_ADR_10] =
		{ GPIOG, { GPIO_PIN_0, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_ADR_11] =
		{ GPIOG, { GPIO_PIN_1, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_CKE1] =
		{ GPIOB, { GPIO_PIN_5, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_NE1] =
		{ GPIOB, { GPIO_PIN_6, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_NWE] =
		{ GPIOC, { GPIO_PIN_0, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_NBL0] =
		{ GPIOE, { GPIO_PIN_0, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_NBL1] =
		{ GPIOE, { GPIO_PIN_1, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_NRAS] =
		{ GPIOF, { GPIO_PIN_11, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_BA0] =
		{ GPIOG, { GPIO_PIN_4, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_BA1] =
		{ GPIOG, { GPIO_PIN_5, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_CLK] =
		{ GPIOG, { GPIO_PIN_7, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },
	[GPIO_SDRAM_NCAS] =
		{ GPIOG, { GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF12_FMC } },

	[GPIO_USB_OTG_FS_ID] =
		{ GPIOB, { GPIO_PIN_12, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF10_OTG_HS } },
	[GPIO_USB_VBUS_FS] =
		{ GPIOB, { GPIO_PIN_13, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF10_OTG_HS } },
	[GPIO_USB_OTG_FS_DM] =
		{ GPIOB, { GPIO_PIN_14, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF10_OTG_HS } },
	[GPIO_USB_OTG_FS_DP] =
		{ GPIOB, { GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF10_OTG_HS } },
	[GPIO_USB_OTG_FS_PCO] =
		{ GPIOC, { GPIO_PIN_4, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF10_OTG_HS } },
	[GPIO_USB_OTG_FS_OC] =
		{ GPIOC, { GPIO_PIN_5, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF10_OTG_HS } },

	[GPIO_USER_0] =
		{ GPIOA, { GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_1] =
		{ GPIOA, { GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },

	[GPIO_USER_2] =
		{ GPIOB, { GPIO_PIN_2, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_3] =
		{ GPIOB, { GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_4] =
		{ GPIOB, { GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_5] =
		{ GPIOB, { GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },

	[GPIO_USER_6] =
		{ GPIOC, { GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_7] =
		{ GPIOC, { GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_8] =
		{ GPIOC, { GPIO_PIN_8, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_9] =
		{ GPIOC, { GPIO_PIN_11, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_10] =
		{ GPIOC, { GPIO_PIN_12, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_11] =
		{ GPIOC, { GPIO_PIN_13, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },

	[GPIO_USER_12] =
		{ GPIOE, { GPIO_PIN_2, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_13] =
		{ GPIOE, { GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_14] =
		{ GPIOE, { GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_15] =
		{ GPIOE, { GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_16] =
		{ GPIOE, { GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },

	[GPIO_USER_17] =
		{ GPIOF, { GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },

	[GPIO_USER_18] =
		{ GPIOG, { GPIO_PIN_2, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_19] =
		{ GPIOG, { GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
	[GPIO_USER_20] =
		{ GPIOG, { GPIO_PIN_9, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, 0 } },
};

static const size_t s_size = sizeof(s_defCon)/sizeof(*s_defCon);

void BSP_Gpio_Init(void) {
	for (size_t i = 0; i < s_size; i++) {
		const GpioCfg_t* const itm = &s_defCon[i];
		HAL_GPIO_Init(itm->port, (GPIO_InitTypeDef*)&itm->config);
	}
}

void BSP_Gpio_Init_Pin(const Gpio_e pin) {
	assert(pin < s_size);
	HAL_GPIO_Init(s_defCon[pin].port, (GPIO_InitTypeDef*)&s_defCon[pin].config);
}

const GpioCfg_t *const BSP_Gpio_CfgGet(const Gpio_e pin) {
	assert(pin < s_size);
	return &s_defCon[pin];
}
