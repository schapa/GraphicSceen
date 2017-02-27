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

bool GfxMultiShape::Draw() {
	bool drawn = false;
	if (!surface || !visible || !dirty)
		return false;
	surface->fill(0);
	for (size_t i = 0; i < shapes.size(); i++)
		drawn |= shapes[i]->Draw();
	dirty = false;
	return drawn;
}
