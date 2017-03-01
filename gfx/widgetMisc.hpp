/*
 * widgetMisc.hpp
 *
 *  Created on: Mar 1, 2017
 *      Author: shapa
 */

#pragma once

#include "shapeSprite.hpp"

#include "shape.hpp"
#include "sprite.hpp"
#include <vector>

class GfxLineShape : public GfxSpriteShape {
public:
	GfxLineShape() : endX(0), endY(0), width(0) {};
	virtual ~GfxLineShape();

	virtual bool Blend(GfxSurface *surface, const uint16_t& offX = 0, const uint16_t& offY = 0);
private:
	uint16_t endX;
	uint16_t endY;
	uint16_t width;
};
