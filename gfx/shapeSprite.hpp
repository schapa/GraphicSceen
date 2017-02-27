/*
 * shapeSprite.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#pragma once

#include "shape.hpp"
#include "sprite.hpp"
#include <vector>

class GfxSpriteShape : public GfxShape {
public:
	GfxSpriteShape() {};
	virtual ~GfxSpriteShape();

	std::vector<SpriteItem*>& getSprites() { return sprites; };
	virtual void createSurface();
	virtual bool Blend(GfxSurface *surface, const uint16_t& offX = 0, const uint16_t& offY = 0);
protected:
	std::vector<SpriteItem*> sprites;
private:
	virtual bool draw();
};
