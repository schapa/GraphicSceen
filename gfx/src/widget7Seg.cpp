/*
 * widget7Seg.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#include "widget7Seg.hpp"
#include "sprites.hpp"

#include <assert.h>
#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif

enum {
	SPRITE_EMPTY = -3,
	SPRITE_MINUS = -2,
	SPRITE_MINUS_ONE = -1,
};

Gfx7SegShape::Gfx7SegShape(): value(-3) {
	sprites.push_back(
			new SpriteItem(
					2, 0, Sprite_7SegmentA));
	sprites.push_back(
			new SpriteItem(
					12, 1, Sprite_7SegmentB));
	sprites.push_back(
			new SpriteItem(
					12, 15, Sprite_7SegmentC));
	sprites.push_back(
			new SpriteItem(
					2, 26, Sprite_7SegmentD));
	sprites.push_back(
			new SpriteItem(
					0, 15, Sprite_7SegmentE));
	sprites.push_back(
			new SpriteItem(
					0, 1, Sprite_7SegmentF));
	sprites.push_back(
			new SpriteItem(
					3, 13, Sprite_7SegmentG));
}

void Gfx7SegShape::setValue(const int8_t &value) {
	static const uint8_t segmap[] = {
			0x7E,
			0x30,
			0x6D,
			0x79,
			0x33,
			0x5B,
			0x5F,
			0x72,
			0x7F,
			0x7B,
	};
	uint8_t mask = 0;
	this->value = value;
	if (value < SPRITE_MINUS)
		mask = 0x00;
	else if (value == SPRITE_MINUS)
		mask = 0x01;
	else if (value == SPRITE_MINUS_ONE)
		mask = 0x31;
	else
		mask = segmap[value%10];
	size_t i = 0;
	for (auto it = sprites.begin(); it != sprites.end(); it++, i++)
		(*it)->isVisible = (mask & (0x40>>i));
	DBGMSG_L("Value %d mask 0x%x", value, mask);
	dirty = true;
}

GfxMulti7SegShape::GfxMulti7SegShape(const size_t& size, const size_t& dotPos):
		zeroTrailing(false), size(size), dotPos(dotPos), value(0), dot(NULL) {
	DBGMSG_H("Creating of %d size", size);
	for (size_t i = 0; i < size; i++) {
		Gfx7SegShape *shape = new Gfx7SegShape();
		shape->setX(16 * i);
		shapes.push_back(shape);
	}
	if (dotPos && ((size - dotPos) > 0)) {
		dot = new SpriteItem(16*(size - dotPos) - 1, 26, Sprite_Dot);
		dot->isVisible = true;

		GfxSpriteShape *sprite = new GfxSpriteShape();
		sprite->setVisible(true);
		shapes.push_back(sprite);
	}
}

void GfxMulti7SegShape::setValue(const int32_t &value) {
	uint32_t absVal = value < 0 ? -value : value;
	const size_t last = size - 1;
	DBGMSG_M("Seting value %d %d", value, size);
	size_t i = 0;
if (last == 0)
return;
	for (auto it = shapes.begin(); it != shapes.end(); it++, i++) {
		Gfx7SegShape *shape = static_cast<Gfx7SegShape*>(*it);

		if (zeroTrailing)
			shape->setVisible(true);
		else if (dot && (i <= dotPos))
			shape->setVisible(true);
		else
			shape->setVisible(!!absVal);
		shape->setValue(absVal%10);
		DBGMSG_L("pos %d val %d", last - i, absVal%10);
		absVal /= 10;
	}
	if (!value) {
		static_cast<Gfx7SegShape*>(shapes.back())->setVisible(true);
	} else if (value < 0) {
		const int8_t sig = ((value > -200) && (value < -99)) ? SPRITE_MINUS_ONE : SPRITE_MINUS;
		DBGMSG_L("Negative. pos %d val %d", 0, sig);
		static_cast<Gfx7SegShape*>(shapes.front())->setValue(sig);
		static_cast<Gfx7SegShape*>(shapes.front())->setVisible(true);
	}
	this->dirty = true;
}
