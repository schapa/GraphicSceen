/*
 * textWidget.hpp
 *
 *  Created on: May 25, 2016
 *      Author: shapa
 */

#ifndef TEXTWIDGET_HPP_
#define TEXTWIDGET_HPP_

#include "widget.hpp"
#include "shapeText.hpp"

class TextWidget : public GfxTextShape {
public:
	TextWidget() {};
	TextWidget(const FontType &font, const uint8_t &textSize);
	TextWidget(const FontType &font, const uint8_t &textSize, const char *text);
	virtual ~TextWidget() {};
};

#endif /* TEXTWIDGET_HPP_ */
