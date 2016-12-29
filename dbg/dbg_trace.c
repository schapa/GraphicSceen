/*
 * dbg_trace.c
 *
 *  Created on: May 6, 2016
 *      Author: shapa
 */

#include "dbg_trace.h"
#include "system.h"
#include "memman.h"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

static char s_msgBuffer[4096];
static int s_msgBufferSize = sizeof(s_msgBuffer);

void Trace_dataAsync(char *buff, size_t size);

void dbgmsg(const char *color, const char *siverity, const char *file, const char *func, int line, const char *fmt, ...) {
	uint32_t primask = System_Lock();

	int occupied = 0;
	if (line) {
		occupied = snprintf(s_msgBuffer, s_msgBufferSize, "[%4lu.%03lu] %s::%s (%d)%s %s: ",
				System_getUptime(), System_getUptimeMs(), file, func, line, color, siverity);
	} else {
		occupied = snprintf(s_msgBuffer, s_msgBufferSize, "[%4lu.%03lu] %s ",
				System_getUptime(), System_getUptimeMs(), color);
	}
	if (occupied < s_msgBufferSize) {
		va_list ap;
		va_start (ap, fmt);
		occupied += vsnprintf(&s_msgBuffer[occupied], s_msgBufferSize - occupied, fmt, ap);
		va_end (ap);
	}
	if (occupied < s_msgBufferSize) {
		occupied += snprintf(&s_msgBuffer[occupied], s_msgBufferSize - occupied, ANSI_ESC_DEFAULT"\r\n");
	}
	if (occupied > s_msgBufferSize) {
		char *trim = "...";
		size_t size = strlen(trim) + 1;
		snprintf(&s_msgBuffer[s_msgBufferSize-size], size, trim);
	}
	char *newBuff = (char*)MEMMAN_malloc(occupied);
	if (newBuff) {
		memcpy((void*)newBuff, (void*)s_msgBuffer, occupied);
	}
	System_Unlock(primask);

	if (newBuff) {
		Trace_dataAsync(newBuff, occupied);
	}
}
