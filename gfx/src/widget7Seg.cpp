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

Gfx7SegShape::Gfx7SegShape(): GfxSpriteShape(true), value(-3) {
	sprites.push_back(
			new SpriteItem(
					1, 0, Sprite_7SegmentA));
	sprites.push_back(
			new SpriteItem(
					13, 1, Sprite_7SegmentB));
	sprites.push_back(
			new SpriteItem(
					13, 28, Sprite_7SegmentC));
	sprites.push_back(
			new SpriteItem(
					1, 50, Sprite_7SegmentD));
	sprites.push_back(
			new SpriteItem(
					0, 28, Sprite_7SegmentE));
	sprites.push_back(
			new SpriteItem(
					0, 1, Sprite_7SegmentF));
	sprites.push_back(
			new SpriteItem(
					2, 24, Sprite_7SegmentG));
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
	if (value < -2)
		mask = 0x00;
	else if (value == -2)
		mask = 0x01;
	else if (value == -1)
		mask = 0x31;
	else
		mask = segmap[value%10];
	for (size_t i = 0; i < sprites.size(); i++)
		sprites[i]->isVisible = (mask & (0x40>>i));
	DBGMSG_L("Value %d mask 0x%x", value, mask);
	dirty = true;
}

GfxMulti7SegShape::GfxMulti7SegShape(): value(0) {
	const size_t size = 3;
	DBGMSG_H("Creating of %d size", size);
	for (size_t i = 0; i < size; i++) {
		shapes.push_back(new Gfx7SegShape());
		shapes[i]->setX(20 * i);
	}
}

GfxMulti7SegShape::~GfxMulti7SegShape() {
	DBGMSG_H("Destroying");

	for (size_t i = 0; i < shapes.size(); i++)
		delete shapes[i];
	shapes.clear();
}

void GfxMulti7SegShape::setValue(const int32_t &value) {
	uint32_t absVal = value < 0 ? -value : value;
	const size_t last = shapes.size() - 1;
	DBGMSG_M("Seting value %d", value);
	for (size_t i = 0; i < shapes.size(); i++) {
		const size_t pos = last - i;
		shapes[pos]->setVisible(!!absVal);
		shapes[pos]->setValue(absVal%10);
		DBGMSG_L("pos %d val %d", pos, absVal%10);
		absVal /= 10;
	}
	if (value < 0) {
		int8_t sig = ((value > -200) && (value < -99)) ? -1 : -2;
		DBGMSG_L("Negative. pos %d val %d", 0, sig);
		shapes[0]->setValue(sig);
		shapes[0]->setVisible(true);
	}
	this->dirty = true;
}

void GfxMulti7SegShape::setSurface(GfxSurface *surface) {
	assert(surface);
	GfxShape::setSurface(surface);
	for (size_t i = 0; i < shapes.size(); i++)
		shapes[i]->setSurface(surface);
}

bool GfxMulti7SegShape::Draw() {
	bool drawn = false;
	if (!surface || !visible || !dirty)
		return false;
	surface->fill(0);
//	for (size_t i = 0; i < shapes.size(); i++)
//		drawn |= shapes[i]->Draw();
	drawn |= shapes[0]->Draw();
	dirty = false;
	return drawn;
}
