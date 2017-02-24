/*
 * shapeSprite.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#include "shapeSprite.hpp"
#include "sprites.hpp"

GfxSpriteShape::GfxSpriteShape() {
	sprites.push_back(
			SpriteItem(
					0, 0, Sprite_7SegmentHorizontal));
}

GfxSpriteShape::~GfxSpriteShape() {

}

bool GfxSpriteShape::Draw() {
	if (!surface || !visible)
		return false;
	dirty = false;
	return true;
}


