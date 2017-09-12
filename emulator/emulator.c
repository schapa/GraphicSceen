/*
 * emulator.c
 *
 *  Created on: Dec 29, 2016
 *      Author: shapa
 */

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include "canWrapper.h"


ssize_t trace_write(const char* buf, size_t nbyte) {
	return write(STDOUT_FILENO, buf, nbyte);

}


void DiscoLCDInit(uint8_t *buff) {}
void DiscoLCDInitLayer(void) {}


_Bool CAN_write(const CanMsg_t *data) {
	return true;
}

void CAN_handleEvent(const Event_t *event) {

}
