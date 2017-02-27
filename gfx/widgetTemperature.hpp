/*
 * widgetTemperature.hpp
 *
 *  Created on: Feb 27, 2017
 *      Author: shapa
 */

#pragma once

#include "shapeMulti.hpp"
#include "widget7Seg.hpp"

class TemperatureWidget: public GfxMultiShape {
public:
	TemperatureWidget();
	virtual ~TemperatureWidget() {};
private:
	bool isCelsius;
	bool isOnA;
	bool isOnB;
	uint32_t temperature;
};
