/*
 * memman.c
 *
 *  Created on: Apr 21, 2016
 *      Author: shapa
 */

#include <stdlib.h>
#include "system.h"

void * __real_malloc(size_t size);
void __real_free(void *__ptr);

void *__wrap_malloc(size_t size) {
	System_Lock();
	void *ptr = __real_malloc(size);
#ifndef EMULATOR
    extern char _Heap_Limit;
    char _stack_Limit;
	if ((ptr >= (void*)&_stack_Limit) || (ptr >= (void*)&_Heap_Limit))
	    while(1);
#endif
	System_Unlock();
	return ptr;
}

void __wrap_free(void *ptr) {
	System_Lock();
	__real_free(ptr);
	System_Unlock();
}
