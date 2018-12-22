/*
 * DiscoLcd_ili9341.h
 *
 *  Created on: Dec 22, 2018
 *      Author: pavelgamov
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

void DiscoLCDInit();
void DiscoLCDInitLayer(uint8_t layno, uint8_t *buff);
void DiscoLCDSetActiveLayer(uint8_t layno);
void DiscoLCD_setState(_Bool state);

#ifdef __cplusplus
}
#endif
