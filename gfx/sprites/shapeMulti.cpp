/*
 * shapeMulti.cpp
 *
 *  Created on: Feb 27, 2017
 *      Author: shapa
 */


#include "shapeMulti.hpp"

#include <assert.h>
#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif


GfxMultiShape::~GfxMultiShape() {
	DBGMSG_H("Destroying");
	for (size_t i = 0; i < shapes.size(); i++)
		delete shapes[i];
	shapes.clear();
}

void GfxMultiShape::setSurface(GfxSurface *surface) {
	assert(surface);
	GfxShape::setSurface(surface);
	for (size_t i = 0; i < shapes.size(); i++)
		shapes[i]->setSurface(surface);
}

void GfxMultiShape::createSurface() {
	for (size_t i = 0; i < shapes.size(); i++)
		shapes[i]->createSurface();
}

bool GfxMultiShape::Blend(GfxSurface *surface, const uint16_t& offX, const uint16_t& offY) {
	if (!isVisible())
		return false;
	for (size_t i = 0; i < shapes.size(); i++)
		if (shapes[i]->isVisible())
			shapes[i]->Blend(surface, offX + x, offY + y);
	return true;
}

bool GfxMultiShape::draw() {
	bool drawn = false;
//	surface->fill(0);
	for (size_t i = 0; i < shapes.size(); i++)
		drawn |= shapes[i]->Draw();
	dirty = false;
	return drawn;
}
