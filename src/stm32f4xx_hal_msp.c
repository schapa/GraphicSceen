/*
 * hal.c
 *
 *  Created on: Mar 20, 2016
 *      Author: shapa
 */

#include "stm32f4xx_hal.h"

static void initGPIO_USART2(void);
static void initGPIO_UART4(void);
static void initGPIO_CAN(void);

void HAL_USART_MspInit(USART_HandleTypeDef *husart) {

	if (husart->Instance == USART1) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_DMA2_CLK_ENABLE();
	} else if (husart->Instance == USART2) {
		__HAL_RCC_GPIOD_CLK_ENABLE();
		__HAL_RCC_USART2_CLK_ENABLE();
		initGPIO_USART2();
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef *husart) {

	if (husart->Instance == UART4) {
		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_UART4_CLK_ENABLE();
		initGPIO_UART4();
	}
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan) {

	if (hcan->Instance == CAN1) {
		__HAL_RCC_GPIOD_CLK_ENABLE();
		__HAL_RCC_CAN1_CLK_ENABLE();
		initGPIO_CAN();
	}
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		__HAL_RCC_TIM2_CLK_ENABLE();
	}
}

void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd) {
	if (hpcd->Instance == USB_OTG_FS) {
		__HAL_RCC_USB_OTG_FS_CLK_ENABLE();
	} else if (hpcd->Instance == USB_OTG_HS) {
		__HAL_RCC_USB_OTG_HS_CLK_ENABLE();
	}
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
}

void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd) {
	if (hpcd->Instance == USB_OTG_FS) {
		__HAL_RCC_USB_OTG_FS_CLK_DISABLE();
	} else if (hpcd->Instance == USB_OTG_HS) {
		__HAL_RCC_USB_OTG_HS_CLK_DISABLE();
	} else {
		__HAL_RCC_USB_OTG_FS_CLK_DISABLE();
		__HAL_RCC_USB_OTG_HS_CLK_DISABLE();
	}
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi) {
	if (hspi->Instance == SPI5) {
		__HAL_RCC_SPI5_CLK_ENABLE();
	}
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c) {
	if (hi2c->Instance == I2C3) {
		__HAL_RCC_I2C3_CLK_ENABLE();
	}
}

void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc) {
	if (hltdc->Instance == LTDC) {
		RCC_PeriphCLKInitTypeDef phClk;
		__HAL_RCC_PLLSAI_ENABLE();
		__HAL_RCC_LTDC_CLK_ENABLE();
		__HAL_RCC_DMA2D_CLK_ENABLE();
		HAL_RCCEx_GetPeriphCLKConfig(&phClk);
		phClk.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
		phClk.PLLSAIDivR = RCC_PLLSAIDIVR_16;
		HAL_RCCEx_PeriphCLKConfig(&phClk);
	}
}

static void initGPIO_USART2(void) {

	return;
	GPIO_InitTypeDef iface = {
			GPIO_PIN_5 | GPIO_PIN_6,
			GPIO_MODE_AF_PP,
			GPIO_NOPULL,
			GPIO_SPEED_FREQ_HIGH,
			GPIO_AF7_USART2
	};
	HAL_GPIO_Init(GPIOD, &iface);
}

static void initGPIO_UART4(void) {

	return;
	GPIO_InitTypeDef iface = {
			GPIO_PIN_10 | GPIO_PIN_11,
			GPIO_MODE_AF_PP,
			GPIO_NOPULL,
			GPIO_SPEED_FREQ_HIGH,
			GPIO_AF8_UART4
	};
	HAL_GPIO_Init(GPIOC, &iface);
}

static void initGPIO_CAN() {
	return;
	GPIO_InitTypeDef iface = {
			GPIO_PIN_0 | GPIO_PIN_1,
			GPIO_MODE_AF_PP,
			GPIO_NOPULL,
			GPIO_SPEED_FREQ_LOW,
			GPIO_AF9_CAN1
	};
	HAL_GPIO_Init(GPIOD, &iface);
}

