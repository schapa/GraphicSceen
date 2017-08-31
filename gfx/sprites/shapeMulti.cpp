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
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		delete *it;
	shapes.clear();
}

void GfxMultiShape::setSurface(GfxSurface *surface) {
	assert(surface);
	GfxShape::setSurface(surface);
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		(*it)->setSurface(surface);
}

void GfxMultiShape::createSurface() {
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		(*it)->createSurface();
}

bool GfxMultiShape::Blend(GfxSurface *surface, const uint16_t& offX, const uint16_t& offY) {
	if (!isVisible())
		return false;
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		if ((*it)->isVisible())
			(*it)->Blend(surface, offX + x, offY + y);
	return true;
}

bool GfxMultiShape::draw() {
	bool drawn = false;
//	surface->fill(0);
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		drawn |= (*it)->Draw();
	dirty = false;
	return drawn;
}
