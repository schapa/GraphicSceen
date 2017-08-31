/*
 * shapeSprite.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#pragma once

#include "shape.hpp"
#include "sprite.hpp"
#include <list>

class GfxSpriteShape : public GfxShape {
public:
	GfxSpriteShape() {};
	GfxSpriteShape(SpriteItem *item) { sprites.push_back(item); };
	virtual ~GfxSpriteShape();

	std::list<SpriteItem*>& getSprites() { return sprites; };
	virtual void createSurface() {};
	virtual bool Blend(GfxSurface *surface, const uint16_t& offX = 0, const uint16_t& offY = 0);
protected:
	std::list<SpriteItem*> sprites;
private:
	virtual bool draw() { return true; }
};
