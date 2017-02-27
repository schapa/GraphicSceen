/*
 * layer.cpp
 *
 *  Created on: May 25, 2016
 *      Author: shapa
 */

#include "layer.hpp"
#include <string.h>

#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif

void GfxLayer::addShape(GfxShape *shape) {
	if (!shape)
		return;
	shapes.push_back(shape);
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
		shapes[i]->Blend(this);
}


void GfxLayer::drawPixel(const uint16_t &x, const uint16_t &y, const uint8_t &alpha) {
	if (!tranparentBlend)
		GfxSurface::drawPixel(x,y, alpha);
	else {
		const uint32_t val = getPixel(x, y);
		if (val < alpha)
			GfxSurface::drawPixel(x,y, (uint32_t)alpha);
	}
}

void GfxLayer::drawPixel(const uint16_t &x, const uint16_t &y, const uint32_t &argb) {
	if (!tranparentBlend)
		GfxSurface::drawPixel(x,y, argb);
	else if (argb < 255)
		drawPixel(x,y, (uint8_t) argb);
	else {
		const uint32_t val = getPixel(x, y) & 0xFF000000;
		if (val < (argb & 0xFF000000)) // compare only by alpha
			GfxSurface::drawPixel(x,y, argb);
	}
}
