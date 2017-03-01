/*
 * shapeText.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#pragma once

#include "shape.hpp"
#include "fontPainter.h"

class GfxTextShape : public GfxShape {
public:
	GfxTextShape();
	virtual ~GfxTextShape();

	const FontType &getFont() { return font; }
	const uint8_t &getFontSize() { return textSize; }
	const char *getText() { return text; }

	void setFont(const FontType &font);
	void setFontSize(const uint8_t &size);
	void setText(const char *text);

	virtual void createSurface();
	virtual bool Blend(GfxSurface *surface, const uint16_t& offX = 0, const uint16_t& offY = 0);

private:
	void renderGrayScale(fontItem_p font, const char *text);
private:
	FontType font;
	uint8_t textSize;
	const char *text;

	virtual bool draw();
};

