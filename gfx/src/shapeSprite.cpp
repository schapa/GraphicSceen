/*
 * shapeSprite.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#include "shapeSprite.hpp"
#include "sprite.hpp"

#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif

GfxSpriteShape::~GfxSpriteShape() {
	sprites.clear();
}

bool GfxSpriteShape::Draw() {
	if (!surface || !visible || !dirty)
		return false;
	if (isUniqueOnSurface)
		surface->fill(0);
	const uint16_t sx = isUniqueOnSurface ? 0 : getX();
	const uint16_t sy = isUniqueOnSurface ? 0 : getY();
	for (size_t i = 0; i < sprites.size(); i++) {
		const SpriteItem& item = sprites[i];
		DBGMSG_H("Draw %d. %d x %d. sz %d", i, item.sprite.getWidth(), item.sprite.getHeight(), item.sprite.getSize());
		if (item.isVisible) {
			const uint16_t& ix = sx + item.getX();
			const uint16_t& iy = sy + item.getY();
			const uint16_t& height = item.sprite.getHeight();
			const uint16_t& width = item.sprite.getWidth();
			for (size_t dy = 0; dy < height; dy++)
				for (size_t dx = 0; dx < width; dx++) {
					uint32_t pix =  item.sprite.getPixel(dx, dy);
					DBGMSG_L("Draw %p at %d %d", pix, ix + dx, iy + dy);
					if (!surface->getPixel(ix + dx, iy + dy))
						surface->drawPixel(ix + dx, iy + dy, pix);
				}
		}
	}
	dirty = false;
	return true;
}


