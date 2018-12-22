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
	System_Lock();
	void *ptr = malloc(size);
    extern char _Heap_Limit;
    char _stack_Limit;
    if ((ptr >= (void*)&_stack_Limit) || (ptr >= (void*)&_Heap_Limit)) {
    	while(1) {
    		System_Lock();
    	}
    }
	System_Unlock();
	return ptr;
}

void MEMMAN_free(void *ptr) {
	System_Lock();
	free (ptr);
	System_Unlock();
}
