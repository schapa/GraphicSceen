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

static void initGPIO_LED(void);

void BSP_Init(void) {
	initGPIO_LED();
	System_setStatus(INFORM_IDLE);
	System_setLedControl(BSP_LedGreenSet);

	PWM_Init();
	PWM_Configure(TIM_CHANNEL_1, 30);
//	OLED_GpioInitParallel();
	OLED_GpioInitSpi();
	SSD1322_InitDisplay();
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
