/*
 * SubiClcokScreen.hpp
 *
 *  Created on: Jun 16, 2017
 *      Author: shapa
 */

#pragma once

#include "surface.hpp"
#include "layer.hpp"
#include "shape.hpp"
#include "widgetText.hpp"
#include "widget7Seg.hpp"
#include "widgetTemperature.hpp"
#include "widgetTrip.hpp"
#include "widgetTime.hpp"
#include "widgetMpg.hpp"

class SubiClcokScreen {

public:
	SubiClcokScreen(const bool &isOled = true) : isOled(isOled) {
		baseLayer = isOled
				? new GfxLayer(PixelFormat_GrayScale, 256, 64)
				: new GfxLayer(PixelFormat_RGB565, 240, 64);
		temperature = new TemperatureWidget();
		trip = new TripWidget();
		mpg = new MPGWidget();
		clock = new TimeWidget();
		init();
	}
	virtual ~SubiClcokScreen() {
		delete baseLayer;
		delete temperature;
		delete trip;
		delete mpg;
		delete clock;
	}

	virtual void draw();

	void setTemperature(const int32_t& val) { temperature->setTemperature(val); }
	void setTemperatureType(const bool& isCelsius) {  temperature->setTemperatureType(isCelsius); }
	void setTripInd(const bool& isOnA, const bool& isOnB) {	trip->setTripInd(isOnA, isOnB); }
	void setValue(const uint16_t& value) { trip->setValue(value); }
	void setType(const MPGWidget::Type& type) { mpg->setType(type); }
	void setTime(const uint8_t& hours, const uint8_t& minutes) { clock->setTime(hours, minutes); }
	void setTimeFormat(const bool& is24h) {  clock->setTimeFormat(is24h); }

private:
	const bool isOled;
	GfxLayer *baseLayer;
	TemperatureWidget *temperature;
	TripWidget *trip;
	MPGWidget *mpg;
	TimeWidget *clock;

private:
	void init();

};
