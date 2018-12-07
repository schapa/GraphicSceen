/*
 * timers.c
 *
 *	Created on: Oct 15, 2016
 *		Author: shapa
 */

#include "timers.h"
#include "system.h"
#include <string.h>

// using uint32_t
#define TIMERS_BITS_COUNT 32

#ifndef LIB_TIMERS_COUNT
#define LIB_TIMERS_COUNT 32
#endif

typedef uint32_t Flags_t;
typedef enum {
	IS_ACTIVE = (1<<0),
	IS_PERIODIC = (1<<1),

	IS_IN_CALLBACK = (1<<16),
} timerFlags_t;

typedef struct {
	uint32_t id;
	uint32_t timeout;
	Flags_t flags;
	onTimerFire_t cb;
	void *cbData;
	uint32_t cnt;
} timerNode_t;

static struct {
	timerNode_t timer[LIB_TIMERS_COUNT];
	volatile uint32_t occupied[LIB_TIMERS_COUNT/TIMERS_BITS_COUNT]; // bit-field
	onTimerPush_t pushCb;
} s_timers;


static uint32_t getNewHandle(void);
static uint8_t isTimerHandleUnique(uint32_t handle);
static uint32_t findTimerById(uint32_t handle);
static void freeTimer(uint32_t position);

void Timer_init(onTimerPush_t cb) {
	s_timers.pushCb = cb;
}

uint32_t Timer_new(uint32_t tout, uint8_t isPeriodic, onTimerFire_t cb, void *cbData) {
	uint32_t handle = INVALID_HANDLE;
	uint32_t result = INVALID_HANDLE;
	if (!cb || !tout || !s_timers.pushCb)
		return INVALID_HANDLE;
	do {
		handle = getNewHandle();
	} while (!isTimerHandleUnique(handle));
	System_Lock();
	for (uint32_t byte = 0; byte < LIB_TIMERS_COUNT/32; byte++) {
		if (s_timers.occupied[byte] ^ ~0) {
			// there is place for timer. Find it
			size_t position = 0;
			for (;position < TIMERS_BITS_COUNT; position++) {
				if ((s_timers.occupied[byte] & (1<<position)) == 0)
					break;
			}
			s_timers.occupied[byte] |= 1<<position;
			position += byte * TIMERS_BITS_COUNT;

			s_timers.timer[position].id = handle;
			s_timers.timer[position].timeout = tout;
			s_timers.timer[position].flags = isPeriodic ? IS_PERIODIC : 0;
			s_timers.timer[position].cb = cb;
			s_timers.timer[position].cbData = cbData;
			result = handle;
			break;
		}
	}
	System_Unlock();
	return result;
}

uint32_t Timer_newArmed(uint32_t tout, uint8_t isPeriodic, onTimerFire_t cb, void *cbData) {
	uint32_t handle = Timer_new(tout, isPeriodic, cb, cbData);
	if (handle != INVALID_HANDLE) {
		Timer_rearm(handle);
	}
	return handle;
}

uint32_t Timer_getCnt(uint32_t id) {
	if (id == INVALID_HANDLE)
		return 0;
	System_Lock();
	uint32_t position = findTimerById(id);
	uint32_t result = 0;
	if (position != LIB_TIMERS_COUNT)
		result = s_timers.timer[position].cnt;
	System_Unlock();
	return result;
}

void Timer_rearm(uint32_t id) {
	if (id == INVALID_HANDLE || !s_timers.pushCb)
		return;
	System_Lock();
	uint32_t position = findTimerById(id);
	if (position != LIB_TIMERS_COUNT) {
		s_timers.timer[position].flags |= IS_ACTIVE;
		s_timers.timer[position].cnt = s_timers.timer[position].timeout;
	}
	System_Unlock();
}

void Timer_rearmTimeout(uint32_t id, uint32_t tout) {
	if (id == INVALID_HANDLE || !s_timers.pushCb)
		return;
	System_Lock();
	uint32_t position = findTimerById(id);
	if (position != LIB_TIMERS_COUNT) {
		s_timers.timer[position].flags |= IS_ACTIVE;
		s_timers.timer[position].timeout = tout;
		s_timers.timer[position].cnt = s_timers.timer[position].timeout;
	}
	System_Unlock();
}

void Timer_disarm(uint32_t id) {
	if (id == INVALID_HANDLE)
		return;
	System_Lock();
	uint32_t position = findTimerById(id);
	if (position != LIB_TIMERS_COUNT) {
		s_timers.timer[position].flags &= ~IS_ACTIVE;
	}
	System_Unlock();
}

void Timer_delete(uint32_t id) {
	if (id == INVALID_HANDLE)
		return;

	System_Lock();
	freeTimer(findTimerById(id));
	System_Unlock();
}

void Timer_makeTick(void) {

	if (!s_timers.pushCb)
		return;

	System_Lock();
	for (size_t i = 0; i < LIB_TIMERS_COUNT; i++) {
		if (s_timers.occupied[i/TIMERS_BITS_COUNT] & 1<<(i%TIMERS_BITS_COUNT)) {
			if ((s_timers.timer[i].flags & IS_ACTIVE) && !s_timers.timer[i].cnt--) {
				// drop active flag. Set in callback
				s_timers.timer[i].flags = IS_IN_CALLBACK | (s_timers.timer[i].flags & ~IS_ACTIVE);
                System_Unlock();
				s_timers.pushCb(s_timers.timer[i].id);
                System_Lock();
				if (s_timers.timer[i].flags & IS_PERIODIC) {
					s_timers.timer[i].flags &= ~IS_IN_CALLBACK;
					s_timers.timer[i].flags |= IS_ACTIVE;
					s_timers.timer[i].cnt = s_timers.timer[i].timeout;
				}
			}
		}
	}
	System_Unlock();
}

void Timer_onTimerCb(uint32_t id) {
	if (id == INVALID_HANDLE)
		return;

	System_Lock();
	for (size_t i = 0; i < LIB_TIMERS_COUNT; i++) {
		if (s_timers.timer[i].id == id) {
			if (s_timers.timer[i].cb) {
				System_Unlock();
				s_timers.timer[i].cb(id, s_timers.timer[i].cbData);
				System_Lock();
			}
			s_timers.timer[i].flags &= ~IS_IN_CALLBACK;
			if (!(s_timers.timer[i].flags & (IS_ACTIVE | IS_PERIODIC))) { // not rearmed
				freeTimer(i);
			}
			break;
		}
	}
	System_Unlock();
}

static uint32_t getNewHandle(void) {
	static uint32_t handle = 0;
	if (!handle || !++handle)
		handle = 1;
	return handle;
}

static uint8_t isTimerHandleUnique(uint32_t handle) {

	for (size_t i = 0; i < LIB_TIMERS_COUNT; i++) {
		if (s_timers.occupied[i/TIMERS_BITS_COUNT] & 1<<(i%TIMERS_BITS_COUNT)) {
			if (s_timers.timer[i].id == handle)
				return 0;
		}
	}
	return !0;
}

static uint32_t findTimerById(uint32_t handle) {
	for (size_t i = 0; i < LIB_TIMERS_COUNT; i++) {
		if (s_timers.occupied[i/TIMERS_BITS_COUNT] & 1<<(i%TIMERS_BITS_COUNT)) {
			if (s_timers.timer[i].id == handle)
				return i;
		}
	}
	return LIB_TIMERS_COUNT;
}


static void freeTimer(uint32_t position) {
	if (position >= LIB_TIMERS_COUNT)
		return;
	memset(&s_timers.timer[position], 0, sizeof(timerNode_t));
	s_timers.occupied[position/TIMERS_BITS_COUNT] &= ~(1<<(position%TIMERS_BITS_COUNT));
}
