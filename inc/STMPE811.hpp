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
	explicit STMPE811(I2C_HandleTypeDef *const itm) : iface(itm) {};
	~STMPE811() {}

public:
	void init();
	bool read(uint16_t &x, uint16_t &y, uint16_t &z);

private:
	I2C_HandleTypeDef *const iface;
};
