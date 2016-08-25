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

#if 01
#include "dbg_trace.h"
#endif

static CAN_HandleTypeDef *s_can1Handle = NULL;

HAL_StatusTypeDef CAN_init(CAN_HandleTypeDef *handle) {

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
		handle->pRxMsg = handle->pTxMsg = NULL;
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
	return result;
}

HAL_StatusTypeDef CAN_write(CanTxMsgTypeDef *txMsg) {
	HAL_StatusTypeDef status = HAL_ERROR;
	size_t msgSize = sizeof(CanTxMsgTypeDef);
	do {
		if (!s_can1Handle || !txMsg)
			break;
		MEMMAN_free(s_can1Handle->pTxMsg);
		s_can1Handle->pTxMsg = MEMMAN_malloc(msgSize);
		if (!s_can1Handle->pTxMsg)
			break;
		memcpy(s_can1Handle->pTxMsg, txMsg, msgSize);
		status = HAL_CAN_Transmit_IT(s_can1Handle);
	} while (0);
	return status;
}

void CAN_handleEvent(Event_p event) {

	if (!event)
		return;
	switch (event->subType.can) {
		case ES_CAN_RX: {
			CanRxMsgTypeDef *rx = event->data.can.rxMsg;
			char buffer[256];
			uint32_t id = rx->IDE ? rx->ExtId : rx->StdId;
			int occupied = 0;
			if (rx->RTR) {
				occupied = snprintf(buffer, sizeof(buffer), "RTR [%p]", (void*)id);
			} else {
				occupied = snprintf(buffer, sizeof(buffer), "DLC [%p] = [", (void*)id);
				for (uint32_t i = 0; i < rx->DLC; i++)
					occupied += snprintf(&buffer[occupied], sizeof(buffer) - occupied, " %d", rx->Data[i]);
				occupied += snprintf(&buffer[occupied], sizeof(buffer) - occupied, "] = %lu", rx->DLC);
			}
			DBGMSG_INFO("Can %s", buffer);
			MEMMAN_free(rx);
			} break;
		case ES_CAN_TX: {
			CanTxMsgTypeDef *tx = event->data.can.txMsg;
			DBGMSG_H("tx [%p] ok", tx->IDE ? tx->ExtId : tx->StdId);
			MEMMAN_free(tx);
			} break;
		case ES_CAN_ERROR: {
			CAN_HandleTypeDef *hcan = event->data.can.hCan;
			DBGMSG_ERR("CAN state %p errno %d", HAL_CAN_GetState(hcan), HAL_CAN_GetError(hcan));
//			DBGMSG_ERR("id [%d] state %p errno %d",
//					HELP_getCanIdByHandle(hcan), HAL_CAN_GetState(hcan), HAL_CAN_GetError(hcan));
			break;
		}
	}
}

void HAL_CAN_TxCpltCallback(CAN_HandleTypeDef* hcan) {
	if (hcan && hcan->pTxMsg) {
		Event_t event = { EVENT_CAN, { ES_CAN_TX },
				.data.can = {
						hcan,
						.txMsg = hcan->pTxMsg
				}
		};
		hcan->pTxMsg = NULL;
		BSP_queuePush(&event);
	}
}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan) {
	if (hcan && hcan->pRxMsg) {
		Event_t event = { EVENT_CAN, { ES_CAN_RX },
				.data.can = {
						hcan,
						.rxMsg = hcan->pRxMsg
				}
		};
		hcan->pRxMsg = MEMMAN_malloc(sizeof(*hcan->pRxMsg));
		BSP_queuePush(&event);
		HAL_CAN_Receive_IT(hcan, CAN_FIFO0);
	}
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan) {
	if (hcan) {
		Event_t event = { EVENT_CAN, { ES_CAN_ERROR },
				.data.can.hCan = hcan
		};
		BSP_queuePush(&event);
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

void CAN_EMULATE_RX(void) {
	CanRxMsgTypeDef *msg = MEMMAN_malloc(sizeof(CanRxMsgTypeDef));
	if (msg) {
		Event_t event = { EVENT_CAN, { ES_CAN_RX },
				.data.can = {
						s_can1Handle,
						.rxMsg = msg
				}
		};
		msg->DLC = 8;
		msg->StdId = 0x50;
		msg->IDE = CAN_ID_STD;
		BSP_queuePush(&event);
	}
}
