/*
 * textWidget.cpp
 *
 *  Created on: May 25, 2016
 *      Author: shapa
 */

#include "textWidget.hpp"

TextWidget::TextWidget() {
	shape = new GfxTextShape();
}

TextWidget::TextWidget(const FontType &font, const uint8_t &textSize) : TextWidget() {
	getShape()->setFont(font);
	getShape()->setFontSize(textSize);
}

TextWidget::TextWidget(const FontType &font, const uint8_t &textSize, const char *text) : TextWidget(font, textSize) {
	getShape()->setText(text);
}

TextWidget::~TextWidget() {
	delete shape;
}


GfxPixmapShape::GfxPixmapShape() {

}

GfxPixmapShape::~GfxPixmapShape() {

}
bool GfxPixmapShape::Draw() {
	if (!surface || !visible)
		return false;
	dirty = false;
	return true;
}

SegmentDisplayWidget::SegmentDisplayWidget(Type type): type(type) {
	shape = new GfxTextShape();
}
SegmentDisplayWidget::~SegmentDisplayWidget() {
	delete shape;
}
//if (!font || !text || !surface || !visible)
//	return;
//uint16_t xPos = 0;
//fontLookupItem_p lookup = font->lookup;
//while (*text) {
//	fontLookupItem_t character = lookup[(size_t)*text];
//    for (uint16_t y = 0; (y < character.heigth) && (y < surface->getHeigth()); ++y) {
//    	const uint8_t *pixel = &font->pixelData[character.offset + y * character.width];
//	    for (uint16_t x = 0; (x < character.width) && (x < surface->getWidth()); ++x) {
//	    	surface->drawPixel(x + xPos, y + character.top, (uint8_t)pixel[x]);
//	    }
//    }
//    xPos += character.advance - character.left;
//    if (xPos > surface->getWidth())
//    	break;
//    text++;
//}
