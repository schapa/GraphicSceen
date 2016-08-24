/*
 * bspOledSpi.c
 *
 *  Created on: May 20, 2016
 *      Author: shapa
 */




/*
 * bsp.c
 *
 *  Created on: May 18, 2016
 *      Author: shapa
 */

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include "bsp.h"
#include "system.h"
#include "ssd1322.h"

static SPI_HandleTypeDef s_spi;

static void initGPIO_Oled(void);
static void initSpi_Oled(void);
static void setDataMode(_Bool mode);

void OLED_GpioInitSpi(void) {
	initGPIO_Oled();
	OLED_ResetSpi(false);
	OLED_CsSpi(false);
	initSpi_Oled();
}
void OLED_ResetSpi(_Bool state) {
	GPIO_PinState val = state ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, val);
}
void OLED_CsSpi(_Bool state) {
	GPIO_PinState val = state ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, val);
}
void OLED_WriteSpi(uint8_t *buff, uint16_t size) {
	setDataMode(true);
	HAL_SPI_Transmit(&s_spi, buff, size, 0xFFFF);
}
void OLED_CmdSpi(uint8_t val) {
	setDataMode(false);
	HAL_SPI_Transmit(&s_spi, &val, 1, 0xFFFF);
}

void OLED_drawSurface(uint8_t *line, uint16_t heigth, uint8_t bytesPerLine) {
    uint32_t i = 0;
    uint32_t j = 0;
    const uint8_t offset = 27;
    uint8_t buffer[128];
	SSD1322_SetColumnRange(offset +1, offset + heigth);
	SSD1322_SetRowRange(0, 70);

	OLED_CsSpi(true);
	OLED_CmdSpi(SSD1322_WRITE_RAM);
	for (i = 0; i < heigth; i++) {
		for (j = 0; j < bytesPerLine; j++) {
			buffer[j] = line[i*bytesPerLine + j];
		}
		OLED_WriteSpi(buffer, bytesPerLine);
	}
	OLED_CsSpi(false);
}

static void initGPIO_Oled(void) {

//	CE 	 == PA4 (16)
//	SCLK == PA5 (4)
//	D/C# == PA6 (14)
//	SDIN == PA7 (5)
//	RESET== PF7 (15)

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();

	GPIO_InitTypeDef iface = {
			GPIO_PIN_5 | GPIO_PIN_7,
			GPIO_MODE_AF_PP,
			GPIO_NOPULL,
			GPIO_SPEED_FREQ_VERY_HIGH,
			GPIO_AF5_SPI1
	};
	HAL_GPIO_Init(GPIOA, &iface);

	iface.Pin = GPIO_PIN_4 | GPIO_PIN_6;
	iface.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOA, &iface);

	iface.Pin = GPIO_PIN_7;
	iface.Mode = GPIO_MODE_OUTPUT_PP;
	iface.Alternate = 0;
	HAL_GPIO_Init(GPIOF, &iface);
}

static void initSpi_Oled(void) {
	SPI_InitTypeDef iface = {
			SPI_MODE_MASTER,
			SPI_DIRECTION_1LINE,
			SPI_DATASIZE_8BIT,
			SPI_POLARITY_LOW,
			SPI_PHASE_1EDGE,
			SPI_NSS_SOFT,
			SPI_BAUDRATEPRESCALER_8,//			SPI_BAUDRATEPRESCALER_256,
			SPI_FIRSTBIT_MSB,
			SPI_TIMODE_DISABLE,
			SPI_CRCCALCULATION_DISABLE,
			0xABCD
	};
	s_spi.Instance = SPI1;
	s_spi.Init = iface;
	HAL_StatusTypeDef initResult = HAL_SPI_Init(&s_spi);

	if (initResult != HAL_OK) {
		__HAL_RCC_SPI1_CLK_DISABLE();
		return;
	}
}

static void setDataMode(_Bool mode) {
	GPIO_PinState val = mode ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, val);
}
