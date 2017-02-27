/*
 * layer.hpp
 *
 *  Created on: May 25, 2016
 *      Author: shapa
 */

#ifndef LAYER_HPP_
#define LAYER_HPP_

#include "surface.hpp"
#include "shape.hpp"
#include <vector>

class GfxLayer : public GfxSurface {
public:
	GfxLayer(ColorDepth bitsDepth, uint16_t width, uint16_t heigth) : GfxSurface(bitsDepth, width, heigth), tranparentBlend(true) {};
	GfxLayer(PixelFormat pixFormat, uint16_t width, uint16_t heigth) : GfxSurface(pixFormat, width, heigth), tranparentBlend(true) {};
	virtual ~GfxLayer() { };
public:
	void addShape(GfxShape *shape);
	void render();
	void setTransparentBlendingMode(const bool &mode) { tranparentBlend = mode; }

	virtual void drawPixel(const uint16_t &x, const uint16_t &y, const uint8_t &alpha);
	virtual void drawPixel(const uint16_t &x, const uint16_t &y, const uint32_t &argb);
private:
	void blend();
private:
	std::vector <GfxShape*>shapes;
	bool tranparentBlend;
};



#endif /* LAYER_HPP_ */
