/*
 * shape.cpp
 *
 *  Created on: May 24, 2016
 *      Author: shapa
 */

#include "shape.hpp"
#include "fontPainter.h"

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

bool GfxTextShape::Draw(void) {
	if (!dirty || !surface || !text) {
		return false;
	}

	fontItem_p fontItem = FontPainter_SizeLookup(this->font, textSize);
	if (!fontItem) {
		return false;
	}
	surface->fill(0);
	switch (surface->getBitsDepth()) {
		case ColorDepth_4:
			renderGrayScale4Bit(fontItem, text);
			break;
		default:
			break;
	}
	dirty = false;
	return true;
}

void GfxTextShape::renderGrayScale4Bit(fontItem_p font, const char *text) {
	if (!font || !text || !surface || !visible)
		return;
	uint16_t xPos = 0;
	fontLookupItem_p lookup = font->lookup;
	const uint8_t *pixel = font->pixelData;
	uint8_t **frame = surface->getFrameBuffer();
	while (*text) {
		fontLookupItem_t character = lookup[(size_t)*text];
	    uint16_t y;
	    for (y = 0; (y < character.heigth) && (y < surface->getHeigth()); ++y) {
	    	uint16_t lineCnt = xPos;
	    	uint16_t xStart = character.offset + y * character.width;
		    uint16_t x = 0;
	    	while ((x < character.width) && (x < surface->getWidth())) {
	    		uint8_t pix = pixel[xStart++]>>4;
	    		uint8_t pix2 = (x+1 < character.width) ? pixel[xStart++]>>4 : 0;
	    		frame[y + character.top][lineCnt++] = pix<<4 | pix2;
	    		x += 2;
	    	}
	    }
//	    xPos += character.advance/2 + character.advance%2;
	    xPos += (character.advance - character.left)/2 + (character.advance - character.left)%2;
	    if (xPos > surface->getWidth()/2)
	    	break;
	    text++;
	}
}
