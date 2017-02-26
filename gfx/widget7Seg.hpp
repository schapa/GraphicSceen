/*
 * widget7Seg.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#pragma once


#include "widget.hpp"
#include "shapeSprite.hpp"
#include <vector>


class Gfx7SegShape : public GfxSpriteShape {
public:
	Gfx7SegShape();
	virtual ~Gfx7SegShape() {};

	const int8_t &getValue() const { return value; }
	void setValue(const int8_t &value);
private:
	int8_t value;
};

class GfxMulti7SegShape : public GfxShape {
public:
	GfxMulti7SegShape();
	virtual ~GfxMulti7SegShape();

	const int32_t &getValue() const { return value; }
	void setValue(const int32_t &value);
	void setSurface(GfxSurface *surface);

	virtual bool Draw();
private:
	std::vector<Gfx7SegShape>shapes;
	int32_t value;
};


class SegmentDisplayWidget : public Widget {
public:
	SegmentDisplayWidget();
	virtual ~SegmentDisplayWidget();

	virtual GfxMulti7SegShape *getShape(void) { return static_cast<GfxMulti7SegShape*>(shape); }
	virtual void setSurface(GfxSurface *surface)  { static_cast<GfxMulti7SegShape*>(shape)->setSurface(surface); Widget::setSurface(surface);}

	const int32_t &getValue() const { return static_cast<GfxMulti7SegShape*>(shape)->getValue(); }
	void setValue(const int32_t &value) { static_cast<GfxMulti7SegShape*>(shape)->setValue(value); }
private:
};

