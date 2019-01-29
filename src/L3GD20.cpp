/*
 * L3GD20.cpp
 *
 *  Created on: Jun 17, 2017
 *      Author: shapa
 */

#include "L3GD20.hpp"
#include "bspGpio.h"

#include "dbg_base.h"
#if 01
#include "dbg_trace.h"
#endif

static void writeReg(SPI_HandleTypeDef *const handle, const uint8_t &adr, const uint8_t val) {
//	HAL_I2C_Mem_Write(handle, 0x82, adr, 1, (uint8_t*)&val, 1, 0x0F);
	uint8_t buff[] = { adr, val };
	BSP_Gpio_SetPin(GPIO_MEMS_CS, false);
	HAL_SPI_Transmit(handle, buff, 2, 0x0F);
	BSP_Gpio_SetPin(GPIO_MEMS_CS, true);
}


void L3GD20::init() {
	writeReg(iface, 0, 0);
	writeReg(iface, 0x20, 0x0F);
	writeReg(iface, 0x23, 0x30);
}

bool L3GD20::read() {
	uint8_t buff[7] = { 0x28 |0xC0, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F };
	BSP_Gpio_SetPin(GPIO_MEMS_CS, false);
	HAL_SPI_TransmitReceive(iface, buff, buff, sizeof(buff), 0xFF);
	int16_t *ptr = (int16_t*)(buff + 1);
	x = ptr[0];
	y = ptr[1];
	z = ptr[2];
	BSP_Gpio_SetPin(GPIO_MEMS_CS, true);
	return true;
}
