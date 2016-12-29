/*
 * system.c
 *
 *  Created on: Dec 29, 2016
 *      Author: shapa
 */

#include <unistd.h>


int System_Lock(void) {
	int primask = 0;
	return primask;
}

void System_Unlock(int primask) {
	if (!primask) {
//		__enable_irq();
	}
}


void Trace_dataAsync(char *buff, size_t size) {
	write(STDOUT_FILENO, buff, size);
}
