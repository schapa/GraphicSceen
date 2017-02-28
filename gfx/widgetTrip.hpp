/*
 * widgetTrip.hpp
 *
 *  Created on: Feb 27, 2017
 *      Author: shapa
 */

#pragma once

#include "shapeMulti.hpp"

class TripWidget: public GfxMultiShape {
public:
	TripWidget();
	virtual ~TripWidget() {};

	void setTripInd(const bool& isOnA, const bool& isOnB) {
		this->isOnA = isOnA;
		this->isOnB = isOnB;
		update();
	}
	void setValue(const uint16_t& value) {
		this->value = value;
		update();
	}
private:
	bool isOnA;
	bool isOnB;
	uint16_t value;
	void update();
};
