/*
 * pwmWrapper.h
 *
 *  Created on: Apr 8, 2016
 *      Author: pavelgamov
 */

#ifndef PWMWRAPPER_H_
#define PWMWRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void PWM_Init(void);
void PWM_Configure(uint32_t channel, uint8_t dutPercent);

#ifdef __cplusplus
}
#endif

#endif /* PWMWRAPPER_H_ */
