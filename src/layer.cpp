/*
 * layer.cpp
 *
 *  Created on: May 25, 2016
 *      Author: shapa
 */

#include "layer.hpp"
#include <string.h>

void GfxLayer::addShape(GfxShape *shape) {
	if (!shape)
		return;
	shapes.push_back(shape);
}

void GfxLayer::addWidget(Widget *widget) {
	if (!widget)
		return;
	addShape(widget->getShape());
}

void GfxLayer::render() {
	bool isDrawn = false;
	for (size_t i = 0; i < shapes.size(); i++)
		isDrawn |= shapes[i]->Draw();
	if (isDrawn)
		tranparentBlend ? blendTransparent() : blend();
}

void GfxLayer::blend() {
	fill(0x00);
	for (size_t i = 0; i < shapes.size(); i++) {
		uint8_t **fb = getFrameBuffer();
		GfxSurface *surface = shapes[i]->getSurface();
		uint16_t surfWidth = getWidth()/2;
		uint16_t xStart = shapes[i]->getX()/2;
		uint16_t yStart = shapes[i]->getY();
		uint16_t shpWidth = shapes[i]->getWidth()/2;
		uint16_t shpHeigth = shapes[i]->getHeigth();
		if (!surface || xStart >= surfWidth)
			continue;
		for (size_t y = 0; (y < shpHeigth) && (yStart + y < getHeigth()); y++) {
			uint8_t *start = &fb[yStart + y][xStart];
			uint16_t len = (xStart + shpWidth) <= surfWidth ? surface->getBytesPerLine() :
					(xStart < surfWidth) ? (surfWidth - xStart) : 0;
			memcpy(start, surface->getFrameBuffer()[y], len);
		}
	}
}

void GfxLayer::blendTransparent() {
	fill(0x00);
	for (size_t i = 0; i < shapes.size(); i++) {
		GfxSurface *surface = shapes[i]->getSurface();
		uint16_t surfWidth = getWidth();
		uint16_t xStart = shapes[i]->getX();
		uint16_t yStart = shapes[i]->getY();
		uint16_t shpWidth = shapes[i]->getWidth();
		uint16_t shpHeigth = shapes[i]->getHeigth();
		if (!surface || xStart >= surfWidth)
			continue;
		for (size_t y = 0; (y < shpHeigth) && (yStart + y < getHeigth()); y++) {
			for (size_t x = 0; (x < shpWidth) && (xStart + x < getWidth()); x++) {
				uint8_t pixel = surface->getPixel(x, y);
				if (pixel) {
					drawPixel(xStart + x, yStart + y, pixel);
				}
			}
		}
	}
}

