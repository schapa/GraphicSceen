/*
 * system.c
 *
 *  Created on: Dec 29, 2016
 *      Author: shapa
 */

#include <unistd.h>
#include <pthread.h>
#include "memman.h"

static pthread_mutex_t s_comMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_comCond = PTHREAD_COND_INITIALIZER;

int System_Lock(void) {
	return pthread_mutex_trylock(&s_comMutex);
}

void System_Unlock(int state) {
	if (!state) {
	    pthread_mutex_unlock(&s_comMutex);
	    pthread_cond_broadcast(&s_comCond);
	}
}

void System_Poll(void) {
	pthread_mutex_lock(&s_comMutex);
	pthread_cond_wait(&s_comCond, &s_comMutex);
    pthread_mutex_unlock(&s_comMutex);
}


void Trace_dataAsync(char *buff, size_t size) {
	write(STDOUT_FILENO, buff, size);
	MEMMAN_free(buff);
}
