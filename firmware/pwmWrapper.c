/*
 * pwmWrapper.c
 *
 *  Created on: Apr 7, 2016
 *      Author: pavelgamov
 */

#include "pwmWrapper.h"

#include "diag/Trace.h"
#include "stm32f4xx_hal.h"
#include <math.h>

static void timerPwmInit(TIM_HandleTypeDef *htim);
static void timerSinInit(TIM_HandleTypeDef *htim);
static void timerOscInit(TIM_HandleTypeDef *htim, uint32_t channel, uint8_t dutPercent);

static TIM_HandleTypeDef s_tim1Handle;
static TIM_HandleTypeDef s_tim2Handle;

void PWM_Init(void) {

	timerPwmInit(&s_tim1Handle);
	timerSinInit(&s_tim2Handle);
	PWM_Configure(TIM_CHANNEL_1, 0);
	PWM_Configure(TIM_CHANNEL_2, 0);
	PWM_Configure(TIM_CHANNEL_3, 0);
	PWM_Configure(TIM_CHANNEL_4, 0);
}

void PWM_Configure(uint32_t channel, uint8_t dutPercent) {

	timerOscInit(&s_tim1Handle, channel, dutPercent);
	HAL_TIM_PWM_Start(&s_tim1Handle, channel);
}

static void timerPwmInit(TIM_HandleTypeDef *htim) {

	if (!htim) {
		return;
	}

	TIM_Base_InitTypeDef iface = {
			0x0,
			TIM_COUNTERMODE_UP,
			0xFF,
			TIM_CLOCKDIVISION_DIV1,
			0
	};
	htim->Init = iface;
	htim->Instance = TIM1;
	HAL_TIM_PWM_Init(htim);
}

static void timerSinInit(TIM_HandleTypeDef *htim) {

	if (!htim) {
		return;
	}

	TIM_Base_InitTypeDef iface = {
			0x0,
			TIM_COUNTERMODE_UP,
			0xFFFF/15,
			TIM_CLOCKDIVISION_DIV4,
			0
	};
	htim->Init = iface;
	htim->Instance = TIM2;
	__HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
	__HAL_TIM_URS_ENABLE(htim);
	HAL_TIM_Base_Init(htim);
	HAL_TIM_Base_Start_IT(htim);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

static void timerOscInit(TIM_HandleTypeDef *htim, uint32_t channel, uint8_t dutPercent) {

	if (!htim || (dutPercent > 100)) {
		return;
	}
	uint16_t dutCycle = htim->Init.Period*dutPercent/255;
	TIM_OC_InitTypeDef sConfigOC = {
			TIM_OCMODE_PWM1,
			dutCycle,
			TIM_OCPOLARITY_HIGH,
			TIM_OCNPOLARITY_LOW,
			TIM_OCFAST_DISABLE,
			TIM_OCIDLESTATE_RESET,
			TIM_OCNIDLESTATE_SET
	};
	HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, channel);
}

static const uint8_t vals[] = {
		0, 12, 24, 36, 48, 59, 70, 80,
		89, 98, 105, 112, 117, 121, 124, 126,
		127, 126, 124, 121, 117, 112, 105, 98,
		89, 80, 70, 59, 48, 36, 24, 12,
		0, -12, -24, -36, -48, -59, -70, -80,
		-89, -98, -105, -112, -117, -121, -124, -126,
		-127, -126, -124, -121, -117, -112, -105, -98,
		-89, -80, -70, -59, -48, -36, -24, -12,
};
static const size_t stepsMax = sizeof(vals)/sizeof(*vals);
static int8_t getVal(const size_t step) {
	if (step > stepsMax)
		return vals[step%stepsMax];
	return vals[step];
}

void TIM2_IRQHandler(void) {
	TIM_HandleTypeDef *htim = &s_tim2Handle;
	__HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
	static uint8_t stepA = 0;
	static uint8_t stepB = stepsMax/4;
	int8_t val = getVal(stepA++);
	if (val > 0) {
		PWM_Configure(TIM_CHANNEL_1, val*2);
		PWM_Configure(TIM_CHANNEL_2, 0);
	} else {
		PWM_Configure(TIM_CHANNEL_1, 0);
		PWM_Configure(TIM_CHANNEL_2, -(val*2));
	}
	val = getVal(stepB++);
	if (val > 0) {
		PWM_Configure(TIM_CHANNEL_3, val*2);
		PWM_Configure(TIM_CHANNEL_4, 0);
	} else {
		PWM_Configure(TIM_CHANNEL_3, 0);
		PWM_Configure(TIM_CHANNEL_4, -(val*2));
	}
}
