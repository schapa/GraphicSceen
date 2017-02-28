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
		sprites.push_back(
				new SpriteItem(
						46, 29, Sprite_Dot));

		setTripAState(true);
		setTripBState(true);
		sprites[2]->isVisible = true;
	};
	virtual ~subiTripInfo() {};

	void setTripAState(const bool& state) { sprites[0]->isVisible = state; }
	void setTripBState(const bool& state) { sprites[1]->isVisible = state; }
};

TripWidget::TripWidget():
		isOnA(false), isOnB(false), value(0) {
	shapes.push_back(new subiTripInfo());
	shapes.push_back(new GfxMulti7SegShape(3));
	shapes[1]->setX(12);
	for (size_t i = 0; i < shapes.size(); i++)
		shapes[i]->setVisible(true);
}


void TripWidget::update() {
	dirty = true;
	subiTripInfo *indication = static_cast<subiTripInfo*>(shapes[0]);
	GfxMulti7SegShape *digits = static_cast<GfxMulti7SegShape*>(shapes[1]);
	indication->setTripAState(isOnA);
	indication->setTripBState(isOnB);
	digits->setValue(value);
}
