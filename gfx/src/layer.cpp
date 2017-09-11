/*
 * layer.cpp
 *
 *  Created on: May 25, 2016
 *      Author: shapa
 */

#include "layer.hpp"
#include <string.h>
#include <algorithm>

#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif

GfxLayer::~GfxLayer() {
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		delete *it;
	shapes.clear();
}

void GfxLayer::addShape(GfxShape *shape) {
	if (!shape)
		return;
	shapes.push_back(shape);
}

void GfxLayer::deleteShape(GfxShape *shape) {

	if (!shape)
		return;
	auto position = std::find(shapes.begin(), shapes.end(), shape);
	if (position != shapes.end())
		shapes.erase(position);
}

void GfxLayer::render(const bool &force) {
	bool isDrawn = false;
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		isDrawn |= (*it)->Draw();
	if (isDrawn || force)
		blend();
}

void GfxLayer::blend() {
	fill(0x00);
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		if ((*it)->isVisible())
			(*it)->Blend(this);
}

void GfxLayer::drawPixel(const uint16_t &x, const uint16_t &y, const uint32_t &argb, const PixelFormat &src) {
	if (!tranparentBlend)
		GfxSurface::drawPixel(x,y, argb, src);
	else {
		const uint32_t old = getPixel(x, y);
		if (old < argb)
			GfxSurface::drawPixel(x,y, argb, src);
	}
}
