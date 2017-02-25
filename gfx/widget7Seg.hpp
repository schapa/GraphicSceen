/*
 * widget7Seg.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#pragma once


#include "widget.hpp"
#include "shapeSprite.hpp"


class Gfx7SegShape : public GfxSpriteShape {
public:
	Gfx7SegShape();
	virtual ~Gfx7SegShape() {};
};


class SegmentDisplayWidget : public Widget {
public:
	enum Type {
		Full,
		Single,
	};
public:
	SegmentDisplayWidget(Type type);
	virtual ~SegmentDisplayWidget();

	const int8_t &getValue() const { return value; }
	void setValue(const int8_t &value);

	virtual GfxSpriteShape *getShape(void) { return static_cast<GfxSpriteShape*>(shape); }
private:
	Type type;
	int8_t value;
};
