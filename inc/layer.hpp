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
#include "textWidget.hpp"
#include <vector>

class GfxLayer : public GfxSurface {
public:
	GfxLayer(ColorDepth bitsDepth, uint16_t width, uint16_t heigth) : GfxSurface(bitsDepth, width, heigth) {};
	virtual ~GfxLayer() { };
public:
	void addShape(GfxShape *shape);
	void addWidget(Widget *widget);
	void render();
private:
	void blend();
private:
	std::vector <GfxShape*>shapes;
};



#endif /* LAYER_HPP_ */
