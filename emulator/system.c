/*
 * system.c
 *
 *  Created on: Dec 29, 2016
 *      Author: shapa
 */

#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include "memman.h"

static pthread_mutex_t s_sysMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t s_condMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_comCond = PTHREAD_COND_INITIALIZER;

static atomic_int s_lock;

void System_Lock(void) {
    int lock = atomic_fetch_add(&s_lock, 1);
    if (!lock) {
        pthread_mutex_lock(&s_sysMutex);
    }
}

void System_Unlock(void) {
    int lock = atomic_fetch_sub(&s_lock, 1);
    if (lock <= 1) {
        pthread_mutex_unlock(&s_sysMutex);
    }
}

void System_Poll(void) {
	pthread_mutex_lock(&s_condMutex);
	pthread_cond_wait(&s_comCond, &s_condMutex);
    pthread_mutex_unlock(&s_condMutex);
}

void System_Wakeup(void) {
    pthread_mutex_lock(&s_sysMutex);
    pthread_cond_broadcast(&s_comCond);
    pthread_mutex_unlock(&s_sysMutex);
}


void Trace_dataAsync(char *buff, size_t size) {
	write(STDOUT_FILENO, buff, size);
	MEMMAN_free(buff);
}
