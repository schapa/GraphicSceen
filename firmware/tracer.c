/*
 * tracer.c
 *
 *  Created on: May 6, 2016
 *      Author: shapa
 */

#include "tracer.h"
#include "string.h"
#include "bsp.h"
#include "memman.h"

typedef enum {
	USART_EVENT_RX,
	USART_EVENT_TX,
	USART_EVENT_RXTX,
	USART_EVENT_ERROR
} UsartEventType_e;

typedef struct {
	UsartEventType_e type;
	void *handle;
} UsartMsg_t;

typedef struct traceNode {
	struct traceNode *next;
	char *string;
	size_t size;
} traceNode_t, *traceNode_p;

static traceNode_p s_traceHead = NULL;
static traceNode_p s_traceTail = NULL;

static USART_HandleTypeDef *s_tracerHandle = NULL;
static DMA_HandleTypeDef *s_tracerTxDmaHandle = NULL;

static HAL_StatusTypeDef initTracerTxDma(DMA_HandleTypeDef *txDmaHandle);
static void sendNextItem(void);

HAL_StatusTypeDef Trace_InitUSART1(USART_HandleTypeDef *handle, DMA_HandleTypeDef *txDmaHandle) {

	HAL_StatusTypeDef result = HAL_ERROR;
	USART_InitTypeDef ifaceParams = {
			115200,
			USART_WORDLENGTH_8B,
			USART_STOPBITS_1,
			USART_PARITY_NONE,
			USART_MODE_TX_RX,
			0,0,0
	};
	if (handle) {
		s_tracerHandle = handle;
		s_tracerTxDmaHandle = txDmaHandle;
		memset(handle, 0, sizeof(*handle));
		handle->Instance = USART1;
		handle->Init = ifaceParams;

		handle->hdmatx = txDmaHandle;
		txDmaHandle->Parent = (void*)handle;

		result = HAL_OK;
		result |= HAL_USART_Init(handle);
		result |= initTracerTxDma(txDmaHandle);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
	return result;
}

void Trace_dataAsync(char *buff, size_t size) {
	_Bool send = false;

	uint32_t primask = __get_PRIMASK();
	__disable_irq();

	traceNode_p elt = (traceNode_p)MEMMAN_malloc(sizeof(traceNode_t));
	if (elt) {
		elt->next = NULL;
		elt->string = buff;
		elt->size = size;
		if (!s_traceHead) {
			s_traceHead = s_traceTail = elt;
			send = true;
		} else {
			s_traceTail->next = elt;
			s_traceTail = elt;
		}
	}
	if (!primask) {
		__enable_irq();
	}

	if (s_tracerHandle && send) {
		sendNextItem();
	}
}

void Trace_dataAsyncFlush(void) {
	if (s_traceHead) {
		HAL_USART_DMAStop(s_tracerHandle);
		while (s_traceHead) {
			HAL_USART_Transmit(s_tracerHandle, (uint8_t*)s_traceHead->string, s_traceHead->size, 0xFF);
			traceNode_p cur = s_traceHead;
			s_traceHead = cur->next;
			MEMMAN_free(cur->string);
			MEMMAN_free(cur);
		}
	}
}

void Trace_dataSync(const char *buff, size_t size) {
	if (!buff || !size)
		return;
	Trace_dataAsyncFlush();
	HAL_USART_Transmit(s_tracerHandle, (uint8_t*)buff, size, 0xFF);
}

_Bool Trace_onTxComplete(USART_HandleTypeDef *handle) {
	_Bool handled = false;
	if (handle == s_tracerHandle && s_traceHead) {
		uint32_t primask = __get_PRIMASK();
		__disable_irq();
		traceNode_p cur = s_traceHead;
		s_traceHead = cur->next;
		MEMMAN_free(cur->string);
		MEMMAN_free(cur);
		if (!s_traceHead) {
			s_traceTail = NULL;
		}
		if (!primask) {
			__enable_irq();
		}
		sendNextItem();
		handled = true;
	}
	return handled;
}

void USART1_IRQHandler(void) {
	HAL_USART_IRQHandler(s_tracerHandle);
}

void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart) {
	if (!Trace_onTxComplete(husart)) {
		EventQueue_Push(
				EVENT_USART,
				NULL, //UsartMsg_t(USART_EVENT_TX, husart, ...)
				NULL);
	}
}

void DMA2_Stream7_IRQHandler(void) {
    HAL_DMA_IRQHandler(s_tracerTxDmaHandle);
}

static HAL_StatusTypeDef initTracerTxDma(DMA_HandleTypeDef *txDmaHandle) {
	HAL_StatusTypeDef result = HAL_ERROR;
	DMA_InitTypeDef ifaceParams = {
			DMA_CHANNEL_4,
			DMA_MEMORY_TO_PERIPH,
			DMA_PINC_DISABLE,
			DMA_MINC_ENABLE,
			DMA_PDATAALIGN_BYTE,
			DMA_MDATAALIGN_BYTE,
			DMA_NORMAL, //DMA_PFCTRL
			DMA_PRIORITY_LOW,
			DMA_FIFOMODE_DISABLE,
			DMA_FIFO_THRESHOLD_1QUARTERFULL,
			DMA_MBURST_SINGLE,
			DMA_PBURST_SINGLE
	};
	if (txDmaHandle) {
		txDmaHandle->Init = ifaceParams;
		txDmaHandle->Instance = DMA2_Stream7;
		result = HAL_DMA_Init(txDmaHandle);
		HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	}
	return result;
}

static void sendNextItem(void) {
	if (s_tracerHandle && s_traceHead) {
		HAL_USART_StateTypeDef state = HAL_USART_GetState(s_tracerHandle);
		if (state != HAL_USART_STATE_BUSY_TX || state != HAL_USART_STATE_BUSY_TX_RX) {
			__HAL_UART_FLUSH_DRREGISTER(s_tracerHandle);
			HAL_USART_Transmit_DMA(s_tracerHandle, (uint8_t*)s_traceHead->string, s_traceHead->size);
		}
	}
}
