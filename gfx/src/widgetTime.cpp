/*
 * widgetTime.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: shapa
 */


#include "widgetTime.hpp"
#include "widget7Seg.hpp"

#include "shapeSprite.hpp"
#include "sprites.hpp"

#include <assert.h>

#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif

class dots : public GfxSpriteShape {
public:
	dots() {
		SpriteItem *dotU = new SpriteItem(0, 0, Sprite_Dot);
		SpriteItem *dotD = new SpriteItem(0, 14, Sprite_Dot);
		sprites.push_back(dotU);
		sprites.push_back(dotD);

		dotU->isVisible = true;
		dotD->isVisible = true;
		setX(31);
		setY(6);
	};
	virtual ~dots() {};
};

TimeWidget::TimeWidget():
		is24h(false), hours(1), minutes(30) {

	GfxMulti7SegShape *h = new GfxMulti7SegShape(2);
	GfxMulti7SegShape *m = new GfxMulti7SegShape(2);
	m->setX(32);
	m->setZeroTrailing(true);
	shapes.push_back(h);
	shapes.push_back(m);
	shapes.push_back(new dots());
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		(*it)->setVisible(true);
}

void TimeWidget::setTime(const uint8_t& hours, const uint8_t& minutes) {
	this->hours = is24h ? hours % 24 : hours % 13;
	this->minutes = minutes % 60;
	update();
}


void TimeWidget::update() {
	dirty = true;
	auto it = shapes.begin();
	GfxMulti7SegShape *h = static_cast<GfxMulti7SegShape*>(*it++);
	GfxMulti7SegShape *m = static_cast<GfxMulti7SegShape*>(*it);
	h->setValue(hours);
	m->setValue(minutes);
}


