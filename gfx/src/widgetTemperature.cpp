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
						0, 17, Sprite_Fahrenheit));

		setCelsiusState(true);
		setFahrenheitState(true);
		setX(53);
	};
	virtual ~subiTempInfo() {};

	void setCelsiusState(const bool& state) { sprites[0]->isVisible = state; }
	void setFahrenheitState(const bool& state) { sprites[1]->isVisible = state; }
};

TemperatureWidget::TemperatureWidget():
		isCelsius(true), temperature(0) {
	shapes.push_back(new GfxMulti7SegShape(3));
	shapes.push_back(new subiTempInfo());
	for (size_t i = 0; i < shapes.size(); i++)
		shapes[i]->setVisible(true);
}


void TemperatureWidget::update() {
	dirty = true;
	GfxMulti7SegShape *digits = static_cast<GfxMulti7SegShape*>(shapes[0]);
	subiTempInfo *indication = static_cast<subiTempInfo*>(shapes[1]);
	digits->setValue(temperature);
	indication->setCelsiusState(isCelsius);
	indication->setFahrenheitState(!isCelsius);
}
