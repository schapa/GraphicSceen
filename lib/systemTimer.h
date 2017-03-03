/*
 * systemStatus.h
 *
 *  Created on: Jan 9, 2016
 *      Author: pavelgamov
 */

#ifndef SYSTEMTIMER_H_
#define SYSTEMTIMER_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	INFORM_INIT,
	INFORM_IDLE,
	INFORM_SLEEP,
	INFORM_CONNECTION_LOST,
	INFORM_ERROR,
	INFORM_LAST
} systemStatus_t;

typedef void (*ledOutputControl_t)(_Bool);

void System_init(ledOutputControl_t control);
void System_setStatus(systemStatus_t);

void System_delayMsDummy(uint32_t delay);
uint32_t System_getUptime(void);
uint32_t System_getUptimeMs(void);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEMTIMER_H_ */

