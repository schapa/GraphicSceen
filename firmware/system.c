/*
 * system.c
 *
 *  Created on: Dec 7, 2016
 *      Author: shapa
 */

#include "system.h"
#include "stm32f4xx.h"

int System_Lock(void) {
	int primask = __get_PRIMASK();
	__disable_irq();
	return primask;
}

void System_Unlock(int primask) {
	if (!primask) {
		__enable_irq();
	}
}

void System_Poll(void) {
    __WFI();
}

#include "dbg_base.h"
void Default_Handler_C(void) {
	const int32_t irq = (__get_IPSR() & 0x0F) - 16;
	if (irq < 0)
		DBGMSG_ERR("System ISR %d", irq + 16);
	else
		DBGMSG_ERR("Unhandled user ISR %d", irq);
}
