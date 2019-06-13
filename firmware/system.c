/*
 * system.c
 *
 *  Created on: Dec 7, 2016
 *      Author: shapa
 */

#include "system.h"
#include "stm32f4xx.h"

#include <stdatomic.h>

static atomic_int s_lock;

void System_Lock(void) {
    int lock = atomic_fetch_add(&s_lock, 1);
    if (!lock) {
        __disable_irq();
    }
}

void System_Unlock(void) {
    int lock = atomic_fetch_sub(&s_lock, 1);
    if (lock <= 1) {
        __enable_irq();
    }
}
void __initialize_hardware_early(void) {

}

void System_Poll(void) {
    __WFE();
}
void System_Wakeup(void) {
    __SEV();
}

#include "dbg_base.h"
void Default_Handler_C(void) {
	const int32_t irq = (__get_IPSR() & 0x0F) - 16;
	if (irq < 0)
		DBGMSG_ERR("System ISR %ld", irq + 16);
	else
		DBGMSG_ERR("Unhandled user ISR %ld", irq);
}
