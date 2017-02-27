/*
 * shapeMulti.hpp
 *
 *  Created on: Feb 27, 2017
 *      Author: shapa
 */

#pragma once

#include "shape.hpp"
#include <vector>

class GfxMultiShape : public GfxShape {
public:
	GfxMultiShape() {};
	virtual ~GfxMultiShape();
	void setSurface(GfxSurface *surface);
	virtual bool Draw();
protected:
	std::vector<GfxShape*>shapes;
};
