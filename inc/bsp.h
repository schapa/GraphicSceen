/*
 * bsp.h
 *
 *  Created on: May 19, 2016
 *      Author: shapa
 */

#ifndef BSP_H_
#define BSP_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#define BSP_TICKS_PER_SECOND 1000

void BSP_Init(void);

void OLED_GpioInitParallel(void);
void OLED_ResetParallel(_Bool state);
void OLED_WriteParallel(uint8_t val);
uint8_t OLED_ReadParallel(void);
void OLED_CmdParallel(uint8_t val);

void OLED_GpioInitSpi(void);
void OLED_ResetSpi(_Bool state);
void OLED_CsSpi(_Bool state);
void OLED_WriteSpi(uint8_t *buff, uint16_t size);
void OLED_CmdSpi(uint8_t val);

void BSP_LedRedSet(FunctionalState state);
void BSP_LedGreenSet(FunctionalState state);

#ifdef __cplusplus
}
#endif


#endif /* BSP_H_ */
