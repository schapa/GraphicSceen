/*
 * STMPE811.hpp
 *
 *  Created on: Jun 15, 2017
 *      Author: shapa
 */

#pragma once

#include "bsp.h"

class STMPE811 {
public:
	explicit STMPE811(I2C_HandleTypeDef *const itm) : iface(itm), x(0), y(0), z(0) {};
	~STMPE811() {}

public:
	void init();
	bool read(bool force=0);

	uint16_t getX() const { return x; }
	uint16_t getY() const { return y; }
	uint16_t getZ() const { return z; }

private:
	I2C_HandleTypeDef *const iface;
	uint16_t x;
	uint16_t y;
	uint16_t z;
};
