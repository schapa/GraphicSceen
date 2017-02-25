/*
 * widget7Seg.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#include "widget7Seg.hpp"

#include "sprites.hpp"

Gfx7SegShape::Gfx7SegShape() {
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

SegmentDisplayWidget::SegmentDisplayWidget(Type type): type(type), value(0) {
	shape = new Gfx7SegShape();
}
SegmentDisplayWidget::~SegmentDisplayWidget() {
	delete shape;
}


void SegmentDisplayWidget::setValue(const int8_t &value) {
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
	std::vector<SpriteItem>& sprites = getShape()->getSprites();
	for (size_t i = 0; i < sprites.size(); i++) {
		sprites[i].isVisible = (mask & (0x40>>i));
	}
	shape->setDirty();
}
