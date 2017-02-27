/*
 * textWidget.cpp
 *
 *  Created on: May 25, 2016
 *      Author: shapa
 */

#include "widgetText.hpp"

TextWidget::TextWidget(const FontType &font, const uint8_t &textSize) : TextWidget() {
	setFont(font);
	setFontSize(textSize);
}

TextWidget::TextWidget(const FontType &font, const uint8_t &textSize, const char *text) : TextWidget(font, textSize) {
	setText(text);
}

