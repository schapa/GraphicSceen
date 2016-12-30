#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "Queue.h"

typedef enum {
	CAN_EVENT_RX,
	CAN_EVENT_TX,
	CAN_EVENT_ERROR,
} CanEventType_e;

typedef struct {
	uint32_t id;
	_Bool isExtended;
	_Bool isRemoteFrame;
	uint8_t size;
	uint8_t buff[8];
} CanMsg_t;

typedef struct {
	CanEventType_e type;
	union {
		uint32_t errCode;
		CanMsg_t *mgs;
	};
} CanMgsEvent_t;

_Bool CAN_init(void *handle);
_Bool CAN_write(const CanMsg_t *data);

void CAN_handleEvent(const Event_t *event);

#ifdef __cplusplus
}
#endif
