/*
 * widgetMisc.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: shapa
 */

#include "widgetMisc.hpp"
#include <stdlib.h>

#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif

void GfxLineShape::drawLine(const uint16_t &sx, const uint16_t &sy, const uint16_t &ex, const uint16_t &ey) {
	if (ex > sx) {
		this->sx = sx;
		this->ex = ex;
	} else {
		this->sx = ex;
		this->ex = sx;
	}
	if (ey > sy) {
		this->sy = sy;
		this->ey = ey;
	} else {
		this->sy = ey;
		this->ey = sy;
	}
	dirty = true;
}

bool GfxLineShape::Blend(GfxSurface *surface, const uint16_t& offX, const uint16_t& offY) {
	const uint16_t dx = ex - sx;
	const uint16_t dy = ey - sy;
	if (!dx && !dy)
		return false;
	const bool byX = dx > dy;
	const uint16_t &length = std::max(dx, dy);
	float div = std::min(dx, dy)/(float)length;

	DBGMSG_H("%d:%d -> %d:%d. len %d, div %f", sx, sy, ex, ey, length, div);

	const uint16_t atX = getX() + offX + sx;
	const uint16_t atY = getY() + offY + sy;
	for (uint16_t i = 0; i < length; i++)
		if (byX)
			surface->drawPixel(atX + i, atY + (div*i), 0xFF, PixelFormat_GrayScale);
		else
			surface->drawPixel(atX + (div*i), atY + i, 0xFF, PixelFormat_GrayScale);
	return true;
}
