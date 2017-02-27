/*
 * shape.hpp
 *
 *  Created on: May 24, 2016
 *      Author: shapa
 */

#ifndef SHAPE_HPP_
#define SHAPE_HPP_

#include "surface.hpp"

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

	const bool &isVisible() const { return visible; }
	void setVisible(const bool& val) { dirty |= visible ^ val; visible = val; }
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

#endif /* SHAPE_HPP_ */
