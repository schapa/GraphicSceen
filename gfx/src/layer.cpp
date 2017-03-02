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
	for (size_t i = 0; i < shapes.size(); i++)
		delete shapes[i];
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
	std::vector <GfxShape*>::iterator position = std::find(shapes.begin(), shapes.end(), shape);
	if (position != shapes.end())
		shapes.erase(position);
}

void GfxLayer::render() {
	bool isDrawn = false;
	for (size_t i = 0; i < shapes.size(); i++)
		isDrawn |= shapes[i]->Draw();
	if (isDrawn)
		blend();
}

void GfxLayer::blend() {
	fill(0x00);
	for (size_t i = 0; i < shapes.size(); i++)
		if (shapes[i]->isVisible())
			shapes[i]->Blend(this);
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
