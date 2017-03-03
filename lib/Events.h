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
	EVENT_TIMCALL,
	EVENT_ADC,
	EVENT_KEY,
	EVENT_USART,
	EVENT_CAN,
	EVENT_STATEMACHINE,
	EVENT_LAST
} EventTypes_e;


typedef void (*onEvtDispose_f) (void *data);

typedef struct {
	EventTypes_e type;
	void *data;
	onEvtDispose_f dispose;
} Event_t;


#ifdef __cplusplus
}
#endif

#endif /* EVENTS_H_ */
