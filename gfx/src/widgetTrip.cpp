/*
 * widgetTemperature.cpp
 *
 *  Created on: Feb 27, 2017
 *      Author: shapa
 */

#include "widgetTrip.hpp"
#include "widget7Seg.hpp"

#include "shapeSprite.hpp"
#include "sprites.hpp"

#include <assert.h>

#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif

class subiTripInfo : public GfxSpriteShape {
public:
	subiTripInfo() {
		sprites.push_back(
				new SpriteItem(
						0, 0, Sprite_TripA));
		sprites.push_back(
				new SpriteItem(
						0, 18, Sprite_TripB));

		setTripAState(true);
		setTripBState(true);
	};
	virtual ~subiTripInfo() {};

	void setTripAState(const bool& state) { sprites.front()->isVisible = state; }
	void setTripBState(const bool& state) { sprites.back()->isVisible = state; }
};

TripWidget::TripWidget():
		isOnA(false), isOnB(false), value(0) {
	GfxMulti7SegShape *tmp = new GfxMulti7SegShape(3, 1);
	shapes.push_back(new subiTripInfo());
	shapes.push_back(tmp);
	tmp->setX(12);
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		(*it)->setVisible(true);
}


void TripWidget::update() {
	dirty = true;
	subiTripInfo *indication = static_cast<subiTripInfo*>(shapes.front());
	GfxMulti7SegShape *digits = static_cast<GfxMulti7SegShape*>(shapes.back());
	indication->setTripAState(isOnA);
	indication->setTripBState(isOnB);
	digits->setValue(value);
}
