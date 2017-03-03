/*
 * timers.h
 *
 *  Created on: Oct 15, 2016
 *      Author: shapa
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define INVALID_HANDLE (0)

typedef void (*onTimerPush_t)(uint32_t id);
typedef void (*onTimerFire_t)(uint32_t id, void *data);

void Timer_init(onTimerPush_t cb);
uint32_t Timer_new(uint32_t tout, uint8_t isPeriodic, onTimerFire_t cb, void *cbData);
uint32_t Timer_newArmed(uint32_t tout, uint8_t isPeriodic, onTimerFire_t cb, void *cbData);
uint32_t Timer_getCnt(uint32_t id);
void Timer_delete(uint32_t id);
void Timer_rearm(uint32_t id);
void Timer_rearmTimeout(uint32_t id, uint32_t tout);
void Timer_disarm(uint32_t id);
void Timer_makeTick(void);

/*
 * @brief Used to call timer callback indirectly
 */
void Timer_onTimerCb(uint32_t id);

#ifdef __cplusplus
}
#endif

#endif /* TIMERS_H_ */
