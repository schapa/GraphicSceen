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
#include "pwmWrapper.h"
#include "canWrapper.h"
#include "tracer.h"

static void initGPIO_LED(void);

static USART_HandleTypeDef s_traceUsart;
static DMA_HandleTypeDef s_traceTxDma;
static CAN_HandleTypeDef s_can1;

_Bool BSP_Init(void) {
	USART_HandleTypeDef *pTraceUsart = &s_traceUsart;
	DMA_HandleTypeDef *pTraceTxDma = &s_traceTxDma;
	HAL_StatusTypeDef initResult = HAL_OK;
	initGPIO_LED();
	System_setStatus(INFORM_IDLE);
	System_setLedControl(BSP_LedGreenSet);

	PWM_Init();
	PWM_Configure(TIM_CHANNEL_1, 15);
	PWM_Configure(TIM_CHANNEL_2, 40);
	PWM_Configure(TIM_CHANNEL_3, 65);
	PWM_Configure(TIM_CHANNEL_4, 90);
	BSP_LcdInit();
	SSD1322_InitDisplay();

	initResult &= Trace_InitUSART1(pTraceUsart, pTraceTxDma);
	initResult &= CAN_init(&s_can1);
	return initResult == HAL_OK;
}

void BSP_LedRedSet(const _Bool state) {
	GPIO_PinState val = state ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, val);
}

void BSP_LedGreenSet(const _Bool state) {
	GPIO_PinState val = state ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, val);
}


static void initGPIO_LED(void) {
	__HAL_RCC_GPIOG_CLK_ENABLE();
	GPIO_InitTypeDef iface = {
			GPIO_PIN_13 | GPIO_PIN_14,
			GPIO_MODE_OUTPUT_PP,
			GPIO_NOPULL,
			GPIO_SPEED_FREQ_LOW,
			0
	};
	HAL_GPIO_Init(GPIOG, &iface);
}
