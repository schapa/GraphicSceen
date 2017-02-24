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

class TextWidget : public Widget {
public:
	TextWidget();
	TextWidget(const FontType &font, const uint8_t &textSize);
	TextWidget(const FontType &font, const uint8_t &textSize, const char *text);
	virtual ~TextWidget();

	const FontType &getFont() { return getShape()->getFont(); }
	const uint8_t &getFontSize() { return getShape()->getFontSize(); }
	const char *getText() { return getShape()->getText(); }

	void setFont(const FontType &font) { getShape()->setFont(font); }
	void setFontSize(const uint8_t &size) { getShape()->setFontSize(size); }
	void setText(const char *text) { getShape()->setText(text); }

	virtual GfxTextShape *getShape(void) { return static_cast<GfxTextShape*>(shape); }
};

#endif /* TEXTWIDGET_HPP_ */
