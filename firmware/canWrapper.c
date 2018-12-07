/*
 * canWrapper.c
 *
 *  Created on: Apr 2, 2016
 *      Author: shapa
 */


#include <string.h>
#include <stdlib.h>

#include "canWrapper.h"
#include "bsp.h"
#include "memman.h"
#include "dbg_base.h"
#include "Queue.h"

#include "stm32f4xx_hal.h"

#if 01
#include "dbg_trace.h"
#endif

static CAN_HandleTypeDef *s_can1Handle = NULL;

static CanMgsEvent_t *newEvent(CanEventType_e type, CanMsg_t *msg, uint32_t errCode);
static void onEventDispose(void *data);

_Bool CAN_init(void *data) {
	CAN_HandleTypeDef *handle = data;

	HAL_StatusTypeDef result = HAL_ERROR;
	const uint32_t baudRate = 125000;

	CAN_InitTypeDef ifaceParams = {
			HAL_RCC_GetPCLK1Freq()/(baudRate*(1+4+3)), //(CAN_SJW + CAN_BS1 + CAN_BS2)
			CAN_MODE_NORMAL,
			CAN_SJW_1TQ,
			CAN_BS1_4TQ,
			CAN_BS2_3TQ,
			.TTCM = DISABLE,
			.ABOM = DISABLE,
			.AWUM = DISABLE,
			.NART = DISABLE,
			.RFLM = DISABLE,
			.TXFP = DISABLE,
	};
	CAN_FilterConfTypeDef filter = {
			0, 0, 0, 0,
			CAN_FIFO0,
			0,
			CAN_FILTERMODE_IDMASK,
			CAN_FILTERSCALE_32BIT,
			ENABLE,
			0
	};

	if (handle) {
		MEMMAN_free(handle->pRxMsg);
		MEMMAN_free(handle->pTxMsg);
		handle->pRxMsg = NULL;
		handle->pTxMsg = NULL;
		memset(handle, 0, sizeof(*handle));
		handle->Instance = CAN1;
		handle->Init = ifaceParams;

		result = HAL_CAN_Init(handle);
		if (result == HAL_OK) {
			HAL_CAN_ConfigFilter(handle, &filter);
			HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
			HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
			HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
//			HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
			handle->pRxMsg = MEMMAN_malloc(sizeof(*handle->pRxMsg));
			result &= HAL_CAN_Receive_IT(handle, CAN_FIFO0);
		}
		s_can1Handle = handle;
	}
	return result == HAL_OK;
}

_Bool CAN_write(const CanMsg_t *data) {
	HAL_StatusTypeDef result = HAL_ERROR;
	static const size_t msgSize = sizeof(CanTxMsgTypeDef);
	if (!data)
		return false;
	CanTxMsgTypeDef txMsg = {
		data->id,
		data->id,
		data->isExtended ? CAN_ID_EXT : CAN_ID_STD,
		data->isRemoteFrame ? CAN_RTR_REMOTE : CAN_RTR_DATA,
		data->isRemoteFrame ? 0 :
				data->size > 8 ? 8 : data->size,
	};

	do {
		if (!s_can1Handle)
			break;
		memcpy(txMsg.Data, data->buff, txMsg.DLC);
		MEMMAN_free(s_can1Handle->pTxMsg);
		s_can1Handle->pTxMsg = MEMMAN_malloc(msgSize);
		if (!s_can1Handle->pTxMsg)
			break;
		memcpy(s_can1Handle->pTxMsg, &txMsg, msgSize);
		result = HAL_CAN_Transmit_IT(s_can1Handle);
	} while (0);
	return result == HAL_OK;
}

void CAN_handleEvent(const Event_t *event) {

	if (!event)
		return;
	CanMgsEvent_t *evt = event->data;
	switch (evt->type) {
		case CAN_EVENT_RX: {
			char buffer[256];
			int occupied = 0;
			if (evt->mgs->isRemoteFrame)
				occupied = snprintf(buffer, sizeof(buffer), "RTR [%p]", (void*)evt->mgs->id);
			else {
				occupied = snprintf(buffer, sizeof(buffer), "DLC [%p] = [", (void*)evt->mgs->id);
				for (uint32_t i = 0; i < evt->mgs->size; i++)
					occupied += snprintf(&buffer[occupied], sizeof(buffer) - occupied, " %d", evt->mgs->buff[i]);
				occupied += snprintf(&buffer[occupied], sizeof(buffer) - occupied, "] = %u", evt->mgs->size);
			}
			DBGMSG_INFO("Can %s", buffer);
			break;
		}
		case CAN_EVENT_TX: {
			DBGMSG_H("tx [%p] ok",  (void*)evt->mgs->id);
			break;
		}
		case CAN_EVENT_ERROR: {
			DBGMSG_ERR("%lu", evt->errCode);
			break;
		}
	}
}

void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan) {
	if (hcan && hcan->pTxMsg) {
		CanMsg_t mgs = {
				hcan->pTxMsg->IDE == CAN_ID_STD ? hcan->pTxMsg->StdId : hcan->pTxMsg->ExtId,
		};
		hcan->pTxMsg = NULL;
		EventQueue_Push(
				EVENT_CAN,
				newEvent(CAN_EVENT_TX, &mgs, HAL_CAN_GetError(hcan)),
				onEventDispose);
	}
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan) {
	if (hcan && hcan->pRxMsg) {

		CanMsg_t mgs = {
				hcan->pRxMsg->IDE == CAN_ID_STD ? hcan->pTxMsg->StdId : hcan->pTxMsg->ExtId,
				hcan->pRxMsg->IDE == CAN_ID_EXT,
				hcan->pTxMsg->RTR == CAN_RTR_REMOTE,
				hcan->pTxMsg->RTR == CAN_RTR_DATA ? hcan->pTxMsg->DLC : 0,
		};
		memcpy(mgs.buff, hcan->pTxMsg->Data, mgs.size);
		EventQueue_Push(
				EVENT_CAN,
				newEvent(CAN_EVENT_RX, &mgs, HAL_CAN_GetError(hcan)),
				onEventDispose);
		HAL_CAN_Receive_IT(hcan, CAN_FIFO0);
	}
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
	if (hcan) {
		EventQueue_Push(
				EVENT_CAN,
				newEvent(CAN_EVENT_ERROR, NULL, HAL_CAN_GetError(hcan)),
				onEventDispose);
	}
}

void CAN1_TX_IRQHandler(void) {
	HAL_CAN_IRQHandler(s_can1Handle);
}
void CAN1_RX0_IRQHandler(void) {
	HAL_CAN_IRQHandler(s_can1Handle);
}
void CAN1_RX1_IRQHandler(void) {
	HAL_CAN_IRQHandler(s_can1Handle);
}
void CAN1_SCE_IRQHandler(void) {
	HAL_CAN_IRQHandler(s_can1Handle);
}


static CanMgsEvent_t *newEvent(CanEventType_e type, CanMsg_t *msg, uint32_t errCode) {
	CanMgsEvent_t *evt = MEMMAN_malloc(sizeof(CanMgsEvent_t));
	if (evt) {
		evt->type = type;
		if (type == CAN_EVENT_ERROR) {
			evt->mgs = NULL;
			evt->errCode = errCode;
		} else {
			evt->mgs = MEMMAN_malloc(sizeof(CanMsg_t));
			if (evt->mgs)
				*evt->mgs = *msg;
			else {
				MEMMAN_free(evt);
				evt = NULL;
			}
		}
	}
	return evt;
}

static void onEventDispose(void *data) {
	CanMgsEvent_t *arg = data;
	MEMMAN_free(arg->mgs);
	arg->mgs = NULL;
	MEMMAN_free(arg);
}

