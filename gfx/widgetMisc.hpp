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

class GfxLineShape : public GfxSpriteShape {
public:
	GfxLineShape() : sx(0), sy(0), ex(0), ey(0), width(1) {};
	virtual ~GfxLineShape() {};

	void drawLine(const uint16_t &sx, const uint16_t &sy, const uint16_t &ex, const uint16_t &ey);
	virtual bool Blend(GfxSurface *surface, const uint16_t& offX = 0, const uint16_t& offY = 0);
private:
	uint16_t sx;
	uint16_t sy;
	uint16_t ex;
	uint16_t ey;
	uint16_t width;
};
