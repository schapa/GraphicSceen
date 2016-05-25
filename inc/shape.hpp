/*
 * shape.hpp
 *
 *  Created on: May 24, 2016
 *      Author: shapa
 */

#ifndef SHAPE_HPP_
#define SHAPE_HPP_

#include "surface.hpp"
#include "fontPainter.h"

class GfxShape {
public:
	GfxShape() {
		surface = NULL;
		alpha = 255;
		visible = false;
		dirty = true;
		x = 0;
		y = 0;
	}
	virtual ~GfxShape() {
		delete surface;
	}

	const bool &isVisible() { return visible; }
	void setVisible(const bool& val) { dirty = visible ^ val; visible = val; }
	void setSurface(GfxSurface *surface) { delete this->surface; this->surface = surface; }
	GfxSurface *getSurface() { return this->surface; }

	const uint16_t &getX(void) { return x; }
	const uint16_t &getY(void) { return y; }
	void setX(const uint16_t &x) {  this->x = x; }
	void setY(const uint16_t &y) {  this->y = y; }
	const uint16_t &getWidth(void) { return surface->getWidth(); }
	const uint16_t &getHeigth(void) { return surface->getHeigth(); }

	virtual bool Draw() = 0;
protected:
	GfxSurface *surface;
	uint8_t alpha;
	bool visible;
	bool dirty;
	uint16_t x;
	uint16_t y;
};

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

	virtual bool Draw();
private:
	void renderGrayScale4Bit(fontItem_p font, const char *text);
private:
	FontType font;
	uint8_t textSize;
	const char *text;
};

#endif /* SHAPE_HPP_ */
