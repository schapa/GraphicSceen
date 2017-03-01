/*
 * widgetTime.hpp
 *
 *  Created on: Feb 27, 2017
 *      Author: shapa
 */

#pragma once

#include "shapeMulti.hpp"

class TimeWidget: public GfxMultiShape {
public:
	TimeWidget();
	virtual ~TimeWidget() {};

	void setTime(const uint8_t& hours, const uint8_t& minutes);
	void setTimeFormat(const bool& is24h) {  this->is24h = is24h; update(); }
private:
	bool is24h;
	uint8_t hours;
	uint8_t minutes;
	void update();
};
