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
	GfxSpriteShape();
	virtual ~GfxSpriteShape();

	std::vector<SpriteItem>& getSprites() { return sprites;};

	virtual bool Draw();
protected:
	std::vector<SpriteItem> sprites;
};
