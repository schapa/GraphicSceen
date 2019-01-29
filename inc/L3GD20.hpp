/*
 * L3GD20.hpp
 *
 *  Created on: Jun 17, 2017
 *      Author: shapa
 */

#pragma once

#include "bsp.h"
class L3GD20 {
public:
	explicit L3GD20(SPI_HandleTypeDef *iface) : iface(iface), x(0), y(0), z(0) {};
	virtual ~L3GD20() {}

	void init();
	bool read();

	int16_t getX() const { return x; }
	int16_t getY() const { return y; }
	int16_t getZ() const { return z; }
private:
	SPI_HandleTypeDef *iface;
	int16_t x;
	int16_t y;
	int16_t z;
};
