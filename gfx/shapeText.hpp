/*
 * shapeText.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#pragma once

#include <string>

#include "shape.hpp"
#include "fontPainter.h"

class GfxTextShape : public GfxShape {
public:
	GfxTextShape();
	virtual ~GfxTextShape();

	const FontType &getFont() const { return font; }
	const uint8_t &getFontSize() const { return textSize; }
	const char *getText() const { return text.c_str(); }
	const size_t getTextWidth() const;
	const size_t getTextHeight() const;

	void setFont(const FontType &font);
	void setFontSize(const uint8_t &size);
	void setText(const char *text);
	void setSpacing(const uint8_t &spacing);
	void setNegative(const bool &negative);

	virtual void createSurface();
	virtual bool Blend(GfxSurface *surface, const uint16_t& offX = 0, const uint16_t& offY = 0);

private:
	void renderGrayScale(fontItem_p font);
private:
	FontType font;
	uint8_t textSize;
	uint8_t spacing;
	std::string text;
	bool negative;

	virtual bool draw();
};

