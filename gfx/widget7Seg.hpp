/*
 * widget7Seg.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#pragma once


#include "widget.hpp"
#include "shapeSprite.hpp"

class SegmentDisplayWidget : public Widget {
public:
	enum Type {
		Full,
		Single,
	};
public:
	SegmentDisplayWidget(Type type);
	virtual ~SegmentDisplayWidget();

	const uint8_t &getValue() { return value; }
	void setValue(const uint8_t &value) { this->value = value; }

	virtual GfxSpriteShape *getShape(void) { return static_cast<GfxSpriteShape*>(shape); }
private:
	Type type;
	uint8_t value;
};
