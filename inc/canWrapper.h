#pragma once

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>
#include "Queue.h"

typedef struct {
	uint32_t id;
	_Bool isExtended;
	_Bool isRemoteFrame;
	size_t size;
	uint8_t buff[8];
} CanMsg_t;

_Bool CAN_init(void *handle);
_Bool CAN_write(const CanMsg_t *data);

void CAN_handleEvent(const Event_p event);

#ifdef __cplusplus
}
#endif
