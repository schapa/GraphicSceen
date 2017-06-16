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
	explicit L3GD20(SPI_HandleTypeDef *iface) : iface(iface) {}
	virtual ~L3GD20() {}

	void init();
	bool read();
private:
	SPI_HandleTypeDef *iface;
};
