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

	virtual bool Draw();
private:
	std::vector<SpriteItem> sprites;
};
