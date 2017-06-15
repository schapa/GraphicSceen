/*
 * STMPE811.c
 *
 *  Created on: Jun 15, 2017
 *      Author: shapa
 */

#include "STMPE811.hpp"

#include "dbg_base.h"
#if 01
#include "dbg_trace.h"
#endif

static void writeReg(I2C_HandleTypeDef *const handle, const uint8_t &adr, const uint8_t val) {
	HAL_I2C_Mem_Write(handle, 0x82, adr, 1, (uint8_t*)&val, 1, 0x0F);
}

static void readReg(I2C_HandleTypeDef *const handle, const uint8_t &adr, uint8_t &val) {
	HAL_I2C_Mem_Read(handle, 0x82, adr, 1, (uint8_t*)&val, 1, 0x0F);
}


void STMPE811::init(void) {
	writeReg(iface, 0x04, 0x0C);
	writeReg(iface, 0x0A, 0x02);
	writeReg(iface, 0x20, 0x40);
	writeReg(iface, 0x21, 0x01);
	writeReg(iface, 0x17, 0x00);
	writeReg(iface, 0x41, 0x9A);
	writeReg(iface, 0x4A, 0x05);
	writeReg(iface, 0x56, 0x07);
	writeReg(iface, 0x58, 0x01);
	writeReg(iface, 0x40, 0x01);
	writeReg(iface, 0x0B, 0xFF);
	writeReg(iface, 0x09, 0x01);
}

bool STMPE811::read(uint16_t &x, uint16_t &y, uint16_t &z) {
	uint8_t interup = 0;
	readReg(iface, 0x0B, interup);
	if (!interup)
		return false;
	DBGMSG_L("isr %p", interup);

	if (interup & 0x03) {
		uint8_t fifo = 0;
		readReg(iface, 0x4C, fifo);
		DBGMSG_L("fifo %d", fifo);
		const size_t size = fifo * 4;
		uint8_t rawData[size];
		HAL_I2C_Mem_Read(iface, 0x82, 0xD7, 1, rawData, size, 0x0F);
		for (size_t i = 0; i < size; i++)
			DBGMSG_L("%d %d", i, rawData[i]);
		writeReg(iface, 0x0B, interup);
		return true;
	}
	if (interup & 0x04) {
		// overflow
		writeReg(iface, 0x0B, 0x04);
	}
	if (interup & 0x08) {
		// fifo full
		writeReg(iface, 0x0B, 0x08);
	}
	if (interup & 0x10) {
		// fifo empty
		writeReg(iface, 0x0B, 0x10);
	}
	if (interup & 0xE0) {
		writeReg(iface, 0x0B, 0xE0);
	}
	return false;
}
