/*
 * bsp.c
 *
 *  Created on: May 18, 2016
 *      Author: shapa
 */

#include <stdbool.h>
#include "bsp.h"
#include "system.h"
#include "ssd1322.h"
#include "pwmWrapper.h"
#include "canWrapper.h"
#include "tracer.h"
#include "bspGpio.h"

#include <assert.h>

static USART_HandleTypeDef s_traceUsart;
static DMA_HandleTypeDef s_traceTxDma;

static CAN_HandleTypeDef s_can1;
static SPI_HandleTypeDef s_spi5;
static I2C_HandleTypeDef s_i2c3;

static void initSpi(SPI_HandleTypeDef *const handle, SPI_TypeDef *const inst);
static void initI2C(I2C_HandleTypeDef *const handle, I2C_TypeDef *const inst);

_Bool BSP_Init(void) {

	System_setStatus(INFORM_LowBatt);
	System_setLedControl(BSP_LedGreenSet);
	BSP_Gpio_Init();
	assert(BSP_SDRAM_Init());
	initSpi(&s_spi5, SPI5);
	initI2C(&s_i2c3, I2C3);

	USART_HandleTypeDef *pTraceUsart = &s_traceUsart;
	DMA_HandleTypeDef *pTraceTxDma = &s_traceTxDma;
	HAL_StatusTypeDef initResult = HAL_OK;

	PWM_Init();
	PWM_Configure(TIM_CHANNEL_1, 30);
	BSP_LcdInit();
	SSD1322_InitDisplay();

	initResult &= Trace_InitUSART1(pTraceUsart, pTraceTxDma);
	if(0)
		initResult &= CAN_init(&s_can1);
	return initResult == HAL_OK;
}

SPI_HandleTypeDef *BSP_GetHandleSpi_5(void) {
	return &s_spi5;
}
I2C_HandleTypeDef *BSP_GetHandleI2C_3(void) {
	return &s_i2c3;
}

void BSP_LedGreenSet(const _Bool state) {
	BSP_Gpio_SetPin(GPIO_LED_GREEN, state);
}

static void initSpi(SPI_HandleTypeDef *const handle, SPI_TypeDef *const inst) {
	if (!handle || !inst)
		return;

	const SPI_InitTypeDef iface = {
		SPI_MODE_MASTER,
		SPI_DIRECTION_2LINES,
		SPI_DATASIZE_8BIT,
		SPI_POLARITY_LOW,
		SPI_PHASE_1EDGE,
		SPI_NSS_SOFT,
		SPI_BAUDRATEPRESCALER_32,//			SPI_BAUDRATEPRESCALER_256,
		SPI_FIRSTBIT_MSB,
		SPI_TIMODE_DISABLE,
		SPI_CRCCALCULATION_DISABLE,
		0xABCD
	};
	handle->Instance = inst;
	handle->Init = iface;
	HAL_SPI_Init(handle);
}

static void initI2C(I2C_HandleTypeDef *const handle, I2C_TypeDef *const inst) {
	if (!handle || !inst)
		return;
	const uint32_t ownAddr = 0x11;
	const I2C_InitTypeDef iface = {
		400 * 1000,
		I2C_DUTYCYCLE_16_9,
		ownAddr,
		I2C_ADDRESSINGMODE_10BIT,
		I2C_DUALADDRESS_DISABLE,
		ownAddr,
		I2C_GENERALCALL_ENABLE,
		I2C_NOSTRETCH_DISABLE, //I2C_NOSTRETCH_ENABLE
	};
	handle->Instance = inst;
	handle->Init = iface;
	HAL_I2C_Init(handle);
}

