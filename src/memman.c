/*
 * memman.c
 *
 *  Created on: Apr 21, 2016
 *      Author: shapa
 */

#include "memman.h"
#include <stdlib.h>
#include "system.h"

void *MEMMAN_malloc(size_t size) {
	int primask = System_Lock();
	void *ptr = malloc(size);
	System_Unlock(primask);
	return ptr;
}

void MEMMAN_free(void *ptr) {
	int primask = System_Lock();
	free (ptr);
	System_Unlock(primask);
}
