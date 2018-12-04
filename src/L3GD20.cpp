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
}

bool L3GD20::read() {
	uint8_t buff[8] = { 0x28 };
	char text[256];
	size_t occup = 0;
	BSP_Gpio_SetPin(GPIO_MEMS_CS, false);
	HAL_SPI_TransmitReceive(iface, buff, buff, sizeof(buff), 0x0F);
	for (size_t i = 0 ; i < sizeof(buff); i++)
		occup += snprintf(text + occup, sizeof(text) - occup, "%3d ", buff[i]);
//	DBGMSG_INFO("\n\r%s", text);
	BSP_Gpio_SetPin(GPIO_MEMS_CS, true);
	return true;
}
