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

#define MAX_TRACE_LEN 4096
#define CHECK(x,y) x > y ? x - y : 0

void Trace_dataAsync(char *buff, size_t size);

void dbgmsg(const char *color, const char *siverity, const char *file, const char *func, int line, const char *fmt, ...) {
	uint32_t primask = System_Lock();

	char *buff = NULL;
	size_t bufferSize = 0;
	do {
		int occupied = 0;
		if (line) {
			occupied = snprintf(buff, bufferSize, "[%4lu.%03lu] %s::%s (%d)%s %s: ",
					System_getUptime(), System_getUptimeMs(), file, func, line, color, siverity);
		} else {
			occupied = snprintf(buff, bufferSize, "[%4lu.%03lu] %s ",
					System_getUptime(), System_getUptimeMs(), color);
		}
		va_list ap;
		va_start (ap, fmt);
		occupied += vsnprintf(buff ? &buff[occupied] : NULL, CHECK(bufferSize, occupied), fmt, ap);
		va_end (ap);
		occupied += snprintf(buff ? &buff[occupied] : NULL, CHECK(bufferSize, occupied), ANSI_ESC_DEFAULT"\r\n");

		if (occupied > MAX_TRACE_LEN)
			occupied = MAX_TRACE_LEN;

		if (!buff) {
			bufferSize = occupied + 1;
			buff = MEMMAN_malloc(bufferSize);
			if (!buff)
				break;
		} else {

			if (occupied >= MAX_TRACE_LEN) {
				char *trim = "...";
				size_t size = strlen(trim) + 1;
				snprintf(&buff[bufferSize-size], size, trim);
				occupied = MAX_TRACE_LEN;
			}
			break;
		}
	} while (true);

	System_Unlock(primask);
	Trace_dataAsync(buff, bufferSize);
}
