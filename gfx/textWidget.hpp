/*
 * textWidget.hpp
 *
 *  Created on: May 25, 2016
 *      Author: shapa
 */

#ifndef TEXTWIDGET_HPP_
#define TEXTWIDGET_HPP_

#include "shape.hpp"
#include "fontPainter.h"

class Widget {
public:
	Widget() : shape(NULL) {};
	virtual ~Widget() { delete shape; };

	const bool &isVisible() { return shape->isVisible(); }
	void setVisible(const bool& val) { shape->setVisible(val); }

	virtual void setSurface(GfxSurface *surface) { getShape()->setSurface(surface); }
	virtual GfxShape *getShape(void) { return shape; }
protected:
	GfxShape *shape;
};

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

class SegmentDisplayWidget : public Widget {
public:
	enum Type {
		Full,
		Single,
	};
public:
	SegmentDisplayWidget(Type type);
	virtual ~SegmentDisplayWidget();

	const uint8_t &getValue() { return value; }
	void setValue(const uint8_t &value) { this->value = value; }
private:
	Type type;
	uint8_t value;
};

#endif /* TEXTWIDGET_HPP_ */
