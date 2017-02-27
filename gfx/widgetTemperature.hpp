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

	void setTemperature(const int32_t& val) { temperature = val;update(); }
	void setTemperatureType(const bool& isCelsius) {  this->isCelsius = isCelsius; update(); }

	void setTripInd(const bool& isOnA, const bool& isOnB) {
		this->isOnA = isOnA;
		this->isOnB = isOnB;
		update();
	}
private:
	bool isCelsius;
	bool isOnA;
	bool isOnB;
	int32_t temperature;
	void update();
};
