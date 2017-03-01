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
	GfxShape():
		surface(NULL),
		alpha(255),
		visible(false),
		dirty(true),
		x(0),
		y(0) {
	}
	virtual ~GfxShape() {
		delete surface;
	}

	const bool &isVisible() const { return visible; }
	virtual void setVisible(const bool& val) { dirty |= visible ^ val; visible = val; }
	virtual void setSurface(GfxSurface *surface) { delete this->surface; this->surface = surface; }
	virtual void createSurface() = 0;
	GfxSurface *getSurface() { return this->surface; }

	const uint16_t &getX(void) { return x; }
	const uint16_t &getY(void) { return y; }
	void setX(const uint16_t &x) {  this->x = x; }
	void setY(const uint16_t &y) {  this->y = y; }
	virtual const uint16_t getWidth(void) const { return surface->getWidth(); }
	virtual const uint16_t getHeight(void) const { return surface->getHeight(); }

	inline bool Draw() {
		if (!visible || !dirty)
			return false;
		bool drawn = draw();
		dirty = false;
		return drawn;
	}
	virtual bool Blend(GfxSurface *surface, const uint16_t& offX = 0, const uint16_t& offY = 0) = 0;
protected:
	GfxSurface *surface;
	uint8_t alpha;
	bool visible;
	bool dirty;
	uint16_t x;
	uint16_t y;
private:
	virtual bool draw() = 0;
};

#endif /* SHAPE_HPP_ */
