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

static volatile EventQueue_p s_eventQueue;
static USART_HandleTypeDef s_traceUsart;
static DMA_HandleTypeDef s_traceTxDma;
static CAN_HandleTypeDef s_can1;

void BSP_Init(void) {
	USART_HandleTypeDef *pTraceUsart = &s_traceUsart;
	DMA_HandleTypeDef *pTraceTxDma = &s_traceTxDma;
	HAL_StatusTypeDef initResult = HAL_OK;
	initGPIO_LED();
	System_setStatus(INFORM_IDLE);
	System_setLedControl(BSP_LedGreenSet);

	PWM_Init();
	PWM_Configure(TIM_CHANNEL_1, 30);
//	OLED_GpioInitParallel();
	OLED_GpioInitSpi();
	SSD1322_InitDisplay();

	initResult &= Trace_InitUSART1(pTraceUsart, pTraceTxDma);
	initResult &= CAN_init(&s_can1);
}

void BSP_queuePush(Event_p pEvent) {
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	s_eventQueue = Queue_pushEvent(s_eventQueue, pEvent);
	if (!primask) {
		__enable_irq();
	}
}

void BSP_pendEvent(Event_p pEvent) {
	while (!s_eventQueue);
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	s_eventQueue = Queue_getEvent(s_eventQueue, pEvent);
	if (!primask) {
		__enable_irq();
	}
}

_Bool BSP_queueIsEventPending(Event_p pEvent) {
	s_eventQueue = Queue_getEvent(s_eventQueue, pEvent);
	return !!s_eventQueue;
}

void BSP_LedRedSet(FunctionalState state) {
	GPIO_PinState val = (state == DISABLE) ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, val);
}

void BSP_LedGreenSet(FunctionalState state) {
	GPIO_PinState val = (state == DISABLE) ? GPIO_PIN_RESET : GPIO_PIN_SET;
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
