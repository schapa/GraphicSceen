/*
 * widgetTemperature.cpp
 *
 *  Created on: Feb 27, 2017
 *      Author: shapa
 */

#include "widgetTemperature.hpp"
#include "widget7Seg.hpp"

#include "shapeSprite.hpp"
#include "sprites.hpp"

#include <assert.h>

#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif

class subiTempInfo : public GfxSpriteShape {
public:
	subiTempInfo() {
		sprites.push_back(
				new SpriteItem(
						0, 2, Sprite_Celsius));
		sprites.push_back(
				new SpriteItem(
						0, 15, Sprite_Fahrenheit));

		setCelsiusState(true);
		setFahrenheitState(true);
		setX(48);
	};
	virtual ~subiTempInfo() {};

	void setCelsiusState(const bool& state) { sprites.front()->isVisible = state; }
	void setFahrenheitState(const bool& state) { sprites.back()->isVisible = state; }
};

TemperatureWidget::TemperatureWidget():
		isCelsius(true), temperature(0) {
	shapes.push_back(new GfxMulti7SegShape(3));
	shapes.push_back(new subiTempInfo());
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		(*it)->setVisible(true);
}


void TemperatureWidget::update() {
	dirty = true;
	GfxMulti7SegShape *digits = static_cast<GfxMulti7SegShape*>(shapes.front());
	subiTempInfo *indication = static_cast<subiTempInfo*>(shapes.back());
	digits->setValue(temperature);
	indication->setCelsiusState(isCelsius);
	indication->setFahrenheitState(!isCelsius);
}
