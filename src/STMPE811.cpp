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


void STMPE811::init() {
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
	read(true);
}

bool STMPE811::read(bool force) {
	uint8_t interup = 0;
	readReg(iface, 0x0B, interup);
	if (force)
		interup = 0x03;
	if (!interup)
		return false;

	if (interup & 0x03) {
		uint8_t fifo = 0;
		readReg(iface, 0x4C, fifo);
		const size_t size = fifo * 4;
		uint8_t rawData[size];
		uint32_t mid_x = 0;
		uint32_t mid_y = 0;
		uint32_t mid_z = 0;
		HAL_I2C_Mem_Read(iface, 0x82, 0xD7, 1, rawData, size, 0x0F);
		for (size_t i = 0; i < fifo; i++) {
			uint8_t *ptr = &rawData[i*4];
			const uint16_t coo_x = ptr[0] << 4 | (ptr[1] >> 4);
			const uint16_t coo_y = (ptr[1] &0x0F) << 8 | (ptr[2]);
			mid_x += coo_x;
			mid_y += coo_y;
			mid_z += ptr[3];
		}
		x = mid_x / fifo;
		y = mid_y / fifo;
		z = mid_z / fifo;
	}
//	if (interup & 0x04) {
//		// overflow
//	}
//	if (interup & 0x08) {
//		// fifo full
//	}
//	if (interup & 0x10) {
//		// fifo empty
//	}
//	if (interup & 0xE0) {
//	}

	writeReg(iface, 0x0B, interup);
	return false;
}
