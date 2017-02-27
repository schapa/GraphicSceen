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

void GfxSpriteShape::createSurface() {
	size_t maxX = 0;
	size_t maxY = 0;
	size_t minX = ~0;
	size_t minY = ~0;
	for (size_t i = 0; i < sprites.size(); i++) {
		const uint16_t spX = sprites[i]->getX();
		const uint16_t spY = sprites[i]->getY();
		const size_t w = spX + sprites[i]->sprite.getWidth();
		const size_t h = spY + sprites[i]->sprite.getHeight();
		maxX = (maxX < w) ? w : maxX;
		maxY = (maxY < h) ? h : maxY;
		minX = (minX > spX) ? spX : minX;
		minY = (minY > spY) ? spY : minY;
	}
	assert(maxX - minX > 0);
	assert(maxY - minY > 0);
	setSurface(new GfxSurface(sprites[0]->sprite.getDepth(), maxX - minX, maxY - minY));
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

bool GfxSpriteShape::draw() {
	surface->fill(0);
	const uint16_t sx = 0;
	const uint16_t sy = 0;
	for (size_t i = 0; i < sprites.size(); i++) {
		SpriteItem* item = sprites[i];
		if (item->isVisible) {
			DBGMSG_H("Item %d", i);
			const uint16_t& ix = sx + item->getX();
			const uint16_t& iy = sy + item->getY();
			const uint16_t& height = item->sprite.getHeight();
			const uint16_t& width = item->sprite.getWidth();
			for (size_t dy = 0; dy < height; dy++)
				for (size_t dx = 0; dx < width; dx++) {
					uint32_t pix =  item->sprite.getPixel(dx, dy);
					DBGMSG_L("Draw %p at %d %d", pix, ix + dx, iy + dy);
					if (!surface->getPixel(ix + dx, iy + dy))
						surface->drawPixel(ix + dx, iy + dy, pix, PixelFormat_GrayScale);
				}
		}
	}
	dirty = false;
	return true;
}


