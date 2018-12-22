/*
 * dbg_trace.c
 *
 *  Created on: May 6, 2016
 *      Author: shapa
 */

#include "dbg_trace.h"
#include "system.h"
#include "memman.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define MAX_TRACE_LEN 4095
#define CHECK(x,y) x > y ? x - y : 0

static char s_buffer[MAX_TRACE_LEN + 1];

void Trace_dataAsync(char *buff, size_t size);
void Trace_dataSync(const char *buff, size_t size) ;

void dbgmsg(const char *color, const char *siverity, const char *file, const char *func, int line, const char *fmt, ...) {
    System_Lock();

    const uint32_t sec = System_getUptime();
    const uint32_t msec = System_getUptimeMs();

    int occupied = 0;
    do {
        if (line) {
            occupied = snprintf(s_buffer, MAX_TRACE_LEN, "[%4lu.%03lu] %s::%s (%d)%s %s: ",
            		sec, msec, file, func, line, color, siverity);
        } else {
            occupied = snprintf(s_buffer, MAX_TRACE_LEN, "[%4lu.%03lu] %s ",
                    sec, msec, color);
        }

        if (occupied > MAX_TRACE_LEN)
            break;
        va_list ap;
        va_start (ap, fmt);
        occupied += vsnprintf(s_buffer + occupied, MAX_TRACE_LEN - occupied, fmt, ap);
        va_end (ap);
        if (occupied > MAX_TRACE_LEN)
            break;

        occupied += snprintf(s_buffer + occupied, MAX_TRACE_LEN - occupied, ANSI_ESC_DEFAULT"\r\n");
    } while (0);
    if (occupied > MAX_TRACE_LEN) {
        occupied = MAX_TRACE_LEN;
        char *trim = "...";
        size_t size = strlen(trim) + 1;
        snprintf(s_buffer + MAX_TRACE_LEN - size, size, trim);
    }
#if 0
    Trace_dataSync(s_buffer, occupied);
    System_Unlock();
#else
    char *buff = MEMMAN_malloc(occupied);
    if (buff)
        memcpy(buff, s_buffer, occupied);

    System_Unlock();
    Trace_dataAsync(buff, occupied);
#endif
}
