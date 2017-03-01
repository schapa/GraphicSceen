/*
 * shape.cpp
 *
 *  Created on: May 24, 2016
 *      Author: shapa
 */

#include "shapeText.hpp"
#include <assert.h>

#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif

GfxTextShape::GfxTextShape(void) {
	font = FONT_DIGITAL_7SEGMENT;
	textSize = 12;
	text = NULL;
}

GfxTextShape::~GfxTextShape(void) {

}

void GfxTextShape::setFont(const FontType &font) {
	dirty = this->font != font;
	this->font = font;
}

void GfxTextShape::setFontSize(const uint8_t &size) {
	dirty = this->textSize != size;
	this->textSize = size;
}

void GfxTextShape::setText(const char *text) {
	dirty = true;
	this->text = text;
}

void GfxTextShape::createSurface() {

	assert(!"Not Implemented");
}

bool GfxTextShape::Blend(GfxSurface *surface, const uint16_t& offX, const uint16_t& offY) {
	if (!isVisible())
		return false;
	DBGMSG_H("Blend %p", this);
	const uint16_t sx = offX + getX();
	const uint16_t sy = offY + getY();
	const uint16_t w = this->surface->getWidth();
	const uint16_t h = this->surface->getHeigth();
	DBGMSG_M("Blend %d. At %d:%d. Sz %dx%d", i, sx, sy, w, h);

	for (size_t y = 0; y < h; y++)
		for (size_t x = 0; x < w; x++)
			surface->drawPixel(sx + x, sy + y, this->surface->getPixel(x, y), PixelFormat_GrayScale);
	return true;
}

bool GfxTextShape::draw(void) {
	if (!text)
		return false;

	fontItem_p fontItem = FontPainter_SizeLookup(this->font, textSize);
	if (!fontItem) {
		return false;
	}
	surface->fill(0);
	switch (surface->getPixelFormat()) {
		case PixelFormat_GrayScale:
			renderGrayScale(fontItem, text);
			break;
		default:
			renderGrayScale(fontItem, text);
			break;
	}
	dirty = false;
	return true;
}

void GfxTextShape::renderGrayScale(fontItem_p font, const char *text) {
	if (!font || !text || !surface || !visible)
		return;
	uint16_t xPos = 0;
	const fontLookupItem_p& lookup = font->lookup;
	const uint16_t &w = surface->getWidth();
	while (*text) {
		fontLookupItem_t character = lookup[(size_t)*text];
	    for (uint16_t y = 0; (y < character.heigth) && (y < surface->getHeigth()); ++y) {
	    	const uint8_t *pixel = &font->pixelData[character.offset + y * character.width];
		    for (uint16_t x = 0; (x < character.width) && (x < surface->getWidth()); ++x) {
		    	surface->drawPixel(x + xPos, y + character.top, (uint8_t)pixel[x], PixelFormat_L8);
		    }
	    }
	    xPos += character.advance - character.left;
	    if (xPos > w)
	    	break;
	    text++;
	}
}
