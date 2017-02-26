/*
 * widget7Seg.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#include "widget7Seg.hpp"

#include "sprites.hpp"

Gfx7SegShape::Gfx7SegShape(): value(-3) {
	sprites.push_back(
			SpriteItem(
					1, 0, Sprite_7SegmentA));
	sprites.push_back(
			SpriteItem(
					13, 1, Sprite_7SegmentB));
	sprites.push_back(
			SpriteItem(
					13, 28, Sprite_7SegmentC));
	sprites.push_back(
			SpriteItem(
					1, 50, Sprite_7SegmentD));
	sprites.push_back(
			SpriteItem(
					0, 28, Sprite_7SegmentE));
	sprites.push_back(
			SpriteItem(
					0, 1, Sprite_7SegmentF));
	sprites.push_back(
			SpriteItem(
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
	for (size_t i = 0; i < sprites.size(); i++) {
		sprites[i].isVisible = (mask & (0x40>>i));
	}
	dirty = true;
}

GfxMulti7SegShape::GfxMulti7SegShape(): value(0) {
	shapes.push_back(Gfx7SegShape());
	shapes.push_back(Gfx7SegShape());
	shapes.push_back(Gfx7SegShape());

	shapes[1].setX(15);
	shapes[2].setX(30);
}

GfxMulti7SegShape::~GfxMulti7SegShape() {
	shapes.clear();
}

void GfxMulti7SegShape::setValue(const int32_t &value) {
	uint32_t absVal = value < 0 ? -value : value;
	const size_t end = shapes.size() - (value < 0);
	for (size_t i = 0 ; i < end; i++) {
		shapes[i].setVisible(true);
		shapes[i].setValue(absVal ?
				absVal%10 : -3);
		absVal /= 10;
	}
	if (value < 0) {
		shapes[end].setValue(-2);
	}
	this->dirty = true;
}

void GfxMulti7SegShape::setSurface(GfxSurface *surface) {
	for (size_t i = 0; i < shapes.size(); i++)
		shapes[i].setSurface(surface);
}

bool GfxMulti7SegShape::Draw() {
	if (!surface || !visible || !dirty)
		return false;
	surface->fill(0);
	for (size_t i = 0; i < shapes.size(); i++) {
//		const Gfx7SegShape& item = shapes[i];
		shapes[i].Draw();
//		DBGMSG_H("Draw %d. %d x %d. sz %d", i, item.sprite.getWidth(), item.sprite.getHeight(), item.sprite.getSize());
//		if (item.isVisible) {
//			const uint16_t& ix = item.getX();
//			const uint16_t& iy = item.getY();
//			const uint16_t& height = item.sprite.getHeight();
//			const uint16_t& width = item.sprite.getWidth();
//			for (size_t dy = 0; dy < height; dy++)
//				for (size_t dx = 0; dx < width; dx++) {
//					uint32_t pix =  item.sprite.getPixel(dx, dy);
//					DBGMSG_L("Draw %p at %d %d", pix, ix + dx, iy + dy);
//					if (!surface->getPixel(ix + dx, iy + dy))
//						surface->drawPixel(ix + dx, iy + dy, pix);
//				}
//		}
	}
	dirty = false;
	return true;
}


SegmentDisplayWidget::SegmentDisplayWidget() {
	shape = new GfxMulti7SegShape();
}

SegmentDisplayWidget::~SegmentDisplayWidget() {
	delete shape;
}

