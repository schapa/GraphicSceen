/*
 * bsp.c
 *
 *  Created on: Dec 29, 2016
 *      Author: shapa
 */


#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#include "bsp.h"
#include "system.h"

extern int pthread_setname_np (pthread_t __target_thread, const char *__name)
	     __THROW __nonnull ((2));

static void* sysTick (void *arg);
static void startSysTick (void);

void BSP_LcdReset(const _Bool state) {

}
void BSP_LcdCs(const _Bool state) {

}
void BSP_LcdWrite(const uint8_t *buff, uint16_t size) {

}
void BSP_LcdCmd(const uint8_t val) {

}

_Bool BSP_Init(void) {
	startSysTick();
	System_setStatus(INFORM_IDLE);

	return true;
}

static void startSysTick (void) {

	pthread_t pid;
	int result = pthread_create(&pid, NULL, sysTick, NULL);
    if (result) {
    	perror("Error - pthread_create()");
    }
	pthread_setname_np(pid, "sysTick");
}

static void* sysTick (void *arg) {
	(void)arg;

	while (true) {
		extern void SysTick_Handler(void);
		extern int usleep (__useconds_t __useconds);
		SysTick_Handler();
		usleep((1000*1000) / BSP_TICKS_PER_SECOND);
	}
	return NULL;
}
