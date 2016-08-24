/*
 * dbg_base.h
 *
 *  Created on: May 6, 2016
 *      Author: shapa
 */

#ifndef TRACER_H_
#define TRACER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <stdbool.h>

HAL_StatusTypeDef Trace_InitUSART1(USART_HandleTypeDef *handle, DMA_HandleTypeDef *txDmaHandle);
_Bool Trace_onTxComplete(USART_HandleTypeDef *handle);
void Trace_dataAsync(char *buff, size_t size);


#ifdef __cplusplus
}
#endif


#endif /* TRACER_H_ */
