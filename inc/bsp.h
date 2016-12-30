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
#include "Queue.h"

#define BSP_TICKS_PER_SECOND 1000

_Bool BSP_Init(void);

void BSP_LedRedSet(const _Bool state);
void BSP_LedGreenSet(const _Bool state);

void BSP_LcdInit(void);
void BSP_LcdReset(const _Bool state);
void BSP_LcdCs(const _Bool state);
void BSP_LcdWrite(const uint8_t *buff, uint16_t size);
void BSP_LcdCmd(const uint8_t val);
void BSP_LcdDrawSurface(const uint8_t *line, const uint16_t heigth, const uint8_t bytesPerLine);


#ifdef __cplusplus
}
#endif

#endif /* BSP_H_ */
