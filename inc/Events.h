/*
 * Events.h
 *
 *  Created on: Apr 20, 2016
 *      Author: shapa
 */

#ifndef EVENTS_H_
#define EVENTS_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	EVENT_DUMMY,
	EVENT_SYSTICK,
	EVENT_KEY,
	EVENT_USART,
	EVENT_CAN,
	EVENT_LAST
} EventTypes_e;


typedef void (*onEvtDispose_f) (void *data);

typedef struct {
	EventTypes_e type;
	void *data;
	onEvtDispose_f dispose;
} Event_t;

//typedef enum {
//	ES_SYSTICK_TICK,
//	ES_SYSTICK_SECOND_ELAPSED
//} SystickSubTypes_t;
//
//typedef enum {
//	ES_EXTI_DOWN,
//	ES_EXTI_UP,
//} ExtiSubTypes_t;
//
//typedef enum {
//	ES_UxART_RX,
//	ES_UxART_TX,
//	ES_UxART_RXTX,
//	ES_UxART_ERROR
//} UxartSubTypes_t;
//typedef struct {
//	union {
//		USART_HandleTypeDef *hUsart;
//		UART_HandleTypeDef *hUart;
//	};
//	intptr_t buffer;
//	size_t size;
//} UxartDataTypes_t;

//
//typedef union {
//	SystickSubTypes_t systick;
//	ExtiSubTypes_t exti;
//	UxartSubTypes_t uxart;
//} EventSubTypes_t;
//
//typedef union {
//	intptr_t intptr;
//	UxartDataTypes_t uxart;
//} EventDataTypes_t;


#ifdef __cplusplus
}
#endif

#endif /* EVENTS_H_ */
