/*
 * widget.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#pragma once

#include "shape.hpp"
#include "surface.hpp"

class Widget {
public:
	Widget() : shape(NULL) {};
	virtual ~Widget() { delete shape; };

	const bool &isVisible() { return shape->isVisible(); }
	void setVisible(const bool& val) { shape->setVisible(val); }

	virtual void setSurface(GfxSurface *surface) { getShape()->setSurface(surface); }
	virtual GfxShape *getShape(void) { return shape; }
protected:
	GfxShape *shape;
};
