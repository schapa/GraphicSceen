/*
 * shape.cpp
 *
 *  Created on: May 24, 2016
 *      Author: shapa
 */

#include "shapeText.hpp"

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
	fontLookupItem_p lookup = font->lookup;
	while (*text) {
		fontLookupItem_t character = lookup[(size_t)*text];
	    for (uint16_t y = 0; (y < character.heigth) && (y < surface->getHeigth()); ++y) {
	    	const uint8_t *pixel = &font->pixelData[character.offset + y * character.width];
		    for (uint16_t x = 0; (x < character.width) && (x < surface->getWidth()); ++x) {
		    	surface->drawPixel(x + xPos, y + character.top, (uint8_t)pixel[x], PixelFormat_GrayScale);
		    }
	    }
	    xPos += character.advance - character.left;
	    if (xPos > surface->getWidth())
	    	break;
	    text++;
	}
}
