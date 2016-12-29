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
