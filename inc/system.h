/*
 * systemStatus.h
 *
 *  Created on: Jan 9, 2016
 *      Author: pavelgamov
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>

typedef enum {
	INFORM_Ok,
	INFORM_LowBatt,
	INFORM_Charging,
	INFORM_Writing,
	INFORM_Reading,
	INFORM_LAST
} systemStatus_t;

typedef void (*ledOutputControl_t)(const _Bool);

void System_setLedControl(ledOutputControl_t);
void System_setStatus(systemStatus_t);

uint32_t System_getUptime(void);
uint32_t System_getUptimeMs(void);

void System_delayMsDummy(uint32_t delay);

void System_Lock(void);
void System_Unlock(void);
void System_Poll(void);
void System_Wakeup(void);

#ifdef __cplusplus
}
#endif


#endif /* SYSTEM_H_ */
