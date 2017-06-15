/*
 * bspOledSpi.c
 *
 *  Created on: May 20, 2016
 *      Author: shapa
 */

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include "bsp.h"
#include "system.h"
#include "ssd1322.h"


static void initGPIO_Oled(void);
static void setDataMode(_Bool mode);

void BSP_LcdInit(void) {
	initGPIO_Oled();
	BSP_LcdReset(false);
	BSP_LcdCs(false);
}
void BSP_LcdReset(_Bool state) {
	GPIO_PinState val = state ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, val);
}
void BSP_LcdCs(_Bool state) {
	GPIO_PinState val = state ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, val);
}
void BSP_LcdWrite(const uint8_t *buff, uint16_t size) {
	setDataMode(true);
	HAL_SPI_Transmit(BSP_GetHandleSpi_5(), (uint8_t*)buff, size, 0xFFFF);
}
void BSP_LcdCmd(uint8_t val) {
	setDataMode(false);
	HAL_SPI_Transmit(BSP_GetHandleSpi_5(), &val, 1, 0xFFFF);
}

static void initGPIO_Oled(void) {

//	SCLK == PF7 (4)    GPIO_SPI5_SCK
//	SDIN == PF9 (5)    GPIO_SPI5_MOSI
//	D/C# == PF6 (14)   GPIO_USER_17
//	RESET== PC3 (15)   GPIO_USER_6
//	CE 	 == PC4 (16)   GPIO_USB_OTG_FS_PCO

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();

	GPIO_InitTypeDef iface = {
			GPIO_PIN_3 | GPIO_PIN_4,
			GPIO_MODE_OUTPUT_PP,
			GPIO_NOPULL,
			GPIO_SPEED_FREQ_VERY_HIGH,
			0
	};
	HAL_GPIO_Init(GPIOC, &iface);

	iface.Pin = GPIO_PIN_6;
	iface.Mode = GPIO_MODE_OUTPUT_PP;
	iface.Alternate = 0;
	HAL_GPIO_Init(GPIOF, &iface);
}

static void setDataMode(_Bool mode) {
	GPIO_PinState val = mode ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, val);
}
