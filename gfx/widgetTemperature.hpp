/*
 * widgetTemperature.hpp
 *
 *  Created on: Feb 27, 2017
 *      Author: shapa
 */

#pragma once

#include "shapeMulti.hpp"

class TemperatureWidget: public GfxMultiShape {
public:
	TemperatureWidget();
	virtual ~TemperatureWidget() {};

	void setTemperature(const int32_t& val) { temperature = val; update(); }
	void setTemperatureType(const bool& isCelsius) {  this->isCelsius = isCelsius; update(); }
private:
	bool isCelsius;
	int32_t temperature;
	void update();
};
