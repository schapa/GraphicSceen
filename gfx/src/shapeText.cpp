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

GfxTextShape::GfxTextShape(void) : font(FONT_LAST), textSize(0), spacing(0), text(NULL), negative(false) {
}

GfxTextShape::~GfxTextShape(void) {

}

const size_t GfxTextShape::getTextWidth() const {
	if (!text || !*text)
		return 0;
	const fontItem_p fontItem = FontPainter_SizeLookup(this->font, textSize);
	if (!fontItem)
		return 0;
	const fontLookupItem_p& lookup = fontItem->lookup;
	const char *text = this->text;
	size_t width = 0;
	DBGMSG_L("[%s]", text);
	while (*text) {
		fontLookupItem_t character = lookup[(size_t)*text];
		width += character.advance - character.left + spacing;
		DBGMSG_L("[%c] a:l:w %d %d %d", *text, character.advance, character.left, character.width);
		text++;
	}
	DBGMSG_L("width %d", width);
	return width;
}

const size_t GfxTextShape::getTextHeight() const {
	if (!text || !*text)
		return 0;
	const fontItem_p fontItem = FontPainter_SizeLookup(this->font, textSize);
	if (!fontItem)
		return 0;
	const fontLookupItem_p& lookup = fontItem->lookup;
	const char *text = this->text;
	size_t height = 0;
	DBGMSG_L("[%s]", text);
	while (*text) {
		fontLookupItem_t character = lookup[(size_t)*text];
		const size_t charHeight = character.heigth + character.top;
		height = charHeight > height ? charHeight : height;
		DBGMSG_L("[%c] t:h %d %d", *text, character.top, character.heigth);
		text++;
	}
	DBGMSG_L("height %d", height);
	return height;
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

void GfxTextShape::setSpacing(const uint8_t &spacing) {
	dirty |= this->spacing ^ spacing;
	this->spacing = spacing;
};

void GfxTextShape::setNegative(const bool &negative) {
	dirty |= this->negative ^ negative;
	this->negative = negative;
}

void GfxTextShape::createSurface() {
	if (!this->text)
		return;
	const size_t w = getTextWidth();
	const size_t h = getTextHeight();

	DBGMSG_L("Creating WxH %dx%d", w, h);
	setSurface(new GfxSurface(PixelFormat_GrayScale, w, h));
}

bool GfxTextShape::Blend(GfxSurface *surface, const uint16_t& offX, const uint16_t& offY) {
	if (!isVisible())
		return false;
	DBGMSG_H("Blend %p", this);
	const uint16_t sx = offX + getX();
	const uint16_t sy = offY + getY();
	const uint16_t w = this->surface->getWidth();
	const uint16_t h = this->surface->getHeight();
	DBGMSG_M("At %d:%d. Sz %dx%d", sx, sy, w, h);

	for (size_t y = 0; y < h; y++)
		for (size_t x = 0; x < w; x++)
			surface->drawPixel(sx + x, sy + y, this->surface->getPixel(x, y), PixelFormat_GrayScale);
	return true;
}

bool GfxTextShape::draw(void) {
	if (!text || !surface)
		return false;

	fontItem_p fontItem = FontPainter_SizeLookup(this->font, textSize);
	if (!fontItem) {
		return false;
	}
	surface->fill(this->negative ? 0xFF : 0);
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
	    for (uint16_t y = 0; (y < character.heigth) && (y < surface->getHeight()); ++y) {
	    	const uint8_t *pixel = &font->pixelData[character.offset + y * character.width];
		    for (uint16_t x = 0; (x < character.width) && (x < surface->getWidth()); ++x) {
		    	const uint32_t pix = this->negative ? 0xFF - pixel[x] : pixel[x];
		    	surface->drawPixel(x + xPos, y + character.top, pix, PixelFormat_L8);
		    }
	    }
	    xPos += character.advance - character.left + spacing;
	    if (xPos > w)
	    	break;
	    text++;
	}
}
