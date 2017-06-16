/*
 * extiHandler.c
 *
 *  Created on: Jun 16, 2017
 *      Author: shapa
 */

#include "stm32f4xx_hal.h"
#include "bspGpio.h"
#include "Queue.h"

void EXTI0_IRQHandler(void) {
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_0)) {
		const Gpio_e pin = GPIO_KEY_WAKE_USER;
		int arg = pin<<1 | BSP_Gpio_ReadPin(pin);
		EventQueue_Push(EVENT_EXTI, (void*)arg, NULL);
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
	}
}

void EXTI1_IRQHandler(void) {
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_1)) {
		const Gpio_e pin = GPIO_MEMS_INT_1;
		int arg = pin<<1 | BSP_Gpio_ReadPin(pin);
		EventQueue_Push(EVENT_EXTI, (void*)arg, NULL);
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
	}
}

void EXTI2_IRQHandler(void) {
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_2)) {
		const Gpio_e pin = GPIO_MEMS_INT_2;
		int arg = pin<<1 | BSP_Gpio_ReadPin(pin);
		EventQueue_Push(EVENT_EXTI, (void*)arg, NULL);
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_2);
	}
}

void EXTI3_IRQHandler(void) {
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3)) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
	}
}

void EXTI4_IRQHandler(void) {
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4)) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
	}
}

void EXTI9_5_IRQHandler(void) {
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_5)) {
		const Gpio_e pin = GPIO_USB_OTG_FS_OC;
		int arg = pin<<1 | BSP_Gpio_ReadPin(pin);
		EventQueue_Push(EVENT_EXTI, (void*)arg, NULL);
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
	}
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_6)) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_6);
	}
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_7)) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);
	}
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8)) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
	}
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_9)) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_9);
	}
}

void EXTI15_10_IRQHandler(void) {

	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_10)) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_10);
	}
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_11)) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_11);
	}
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_12)) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
	}
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13)) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
	}
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14)) {
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);
	}
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_15)) {
		const Gpio_e pin = GPIO_TOUCH_INT;
		int arg = pin<<1 | BSP_Gpio_ReadPin(pin);
		EventQueue_Push(EVENT_EXTI, (void*)arg, NULL);
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_15);
	}
}
