/*
 * shapeSprite.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#include "shapeSprite.hpp"
#include "sprite.hpp"

#include <assert.h>

#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif

GfxSpriteShape::~GfxSpriteShape() {
	for (size_t i = 0; i < sprites.size(); i++)
		delete sprites[i];
	sprites.clear();
}

bool GfxSpriteShape::Blend(GfxSurface *surface, const uint16_t& offX, const uint16_t& offY) {

	if (!isVisible())
		return false;
	DBGMSG_H("Blend %p", this);
	for (size_t i = 0; i < sprites.size(); i++) {
		SpriteItem* item = sprites[i];
		if (!item->isVisible)
			continue;
		const uint16_t sx = offX + getX() + item->getX();
		const uint16_t sy = offY + getY() + item->getY();
		const uint16_t w = item->sprite.getWidth();
		const uint16_t h = item->sprite.getHeight();
		DBGMSG_M("Blend %d. At %d:%d. Sz %dx%d", i, sx, sy, w, h);

		for (size_t y = 0; y < h; y++)
			for (size_t x = 0; x < w; x++)
				surface->drawPixel(sx + x, sy + y, item->sprite.getPixel(x, y), PixelFormat_GrayScale);

	}
	return true;
}


