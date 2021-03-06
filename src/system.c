/*
 * systemTimer.c
 *
 *  Created on: April 2, 2016
 *      Author: pavelgamov
 */

#include <stddef.h>
#include "system.h"
#include "timers.h"
#include "bsp.h"
#include "lvgl.h"

static struct {
	uint32_t activeTime;
	uint32_t passiveTime;
} s_timing[] = {
		[INFORM_Ok] = { 0.1*TICKS_PER_SECOND, 0.3*TICKS_PER_SECOND },
		[INFORM_LowBatt] = { 0.1*TICKS_PER_SECOND, TICKS_PER_SECOND },
		[INFORM_Charging] = { 0.05*TICKS_PER_SECOND, 2*TICKS_PER_SECOND},
		[INFORM_Writing] = { 0.1*TICKS_PER_SECOND, 0.5*TICKS_PER_SECOND},
		[INFORM_Reading] = { 0.05*TICKS_PER_SECOND, 0.05*TICKS_PER_SECOND},
};

static systemStatus_t s_systemStatus = INFORM_Reading;
static uint32_t s_systemStatusTimer = 0;
static ledOutputControl_t s_systemLed = NULL;
static volatile uint32_t s_uptimeSeconds = 0;
static volatile uint32_t s_uptimeTicks = 1;

void System_setLedControl(ledOutputControl_t control) {
	s_systemLed = control;
}

void System_setStatus(systemStatus_t status) {
	if(status < INFORM_LAST) {
		s_systemStatus = status;
	}
}

void SysTick_Handler(void) {
	uint32_t period = s_timing[s_systemStatus].activeTime + s_timing[s_systemStatus].passiveTime;
	if (s_systemLed)
		s_systemLed(s_systemStatusTimer <= s_timing[s_systemStatus].activeTime);
	if (++s_systemStatusTimer > period) {
		s_systemStatusTimer = 0;
	}

	if (!(s_uptimeTicks++ % TICKS_PER_SECOND)) {
		s_uptimeSeconds++;
		EventQueue_Push(EVENT_SYSTICK, (void*)s_uptimeSeconds, NULL);
	}
#if defined(USE_HAL_DRIVER)
	HAL_IncTick();
#endif
	lv_tick_inc(1);
	Timer_makeTick();
}

void System_delayMsDummy(uint32_t delay) {
    delay += s_uptimeTicks;
	while (delay > s_uptimeTicks);
}

uint32_t System_getUptime(void) {
	return s_uptimeSeconds;
}
uint32_t System_getUptimeMs(void) {
	return s_uptimeTicks%TICKS_PER_SECOND;
}
