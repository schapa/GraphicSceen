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
	GfxSpriteShape(bool isSharingSurface = false): isSharingSurface(isSharingSurface) {};
	virtual ~GfxSpriteShape();

	std::vector<SpriteItem*>& getSprites() { return sprites; };

	virtual bool draw();
protected:
	std::vector<SpriteItem*> sprites;
	bool isSharingSurface;
};
