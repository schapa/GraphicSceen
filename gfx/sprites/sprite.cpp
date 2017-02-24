/*
 * sprite.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#include "sprite.hpp"

#include "dbg_base.h"
#if 01
#include "dbg_trace.h"
#endif

const uint32_t Sprite::getPixel(const uint16_t& x, const uint16_t& y) const {
	assert(x < width);
	assert(y < height);
	switch (depth) {
		case ColorDepth_4: {
			const uint8_t*val = static_cast<const uint8_t*>(data);
			const uint32_t result = (x % 2) ?
					val[(y*width + x)/2] >> 4 :
					val[(y*width + x)/2] & 0x0F;

			DBGMSG_M("x:y %d:%d calc off %d + %d = 0x%x", x, y, y*width + x/2, (x % 2), result);
			return result;
			break;
		}
		case ColorDepth_8: {
			const uint8_t*val = static_cast<const uint8_t*>(data);
			return val[y*width + x];
			break;
		}
		case ColorDepth_16: {
			const uint16_t*val = static_cast<const uint16_t*>(data);
			return val[y*width + x];
			break;
		}
		case ColorDepth_24: {
			assert(!"Implement me");
//			const uint8_t*val = static_cast<const uint8_t*>(data);
//			return val[x*width + y/3];
			break;
		}
		case ColorDepth_32: {
			const uint32_t*val = static_cast<const uint32_t*>(data);
			return val[y*width + x];
			break;
		}
	}
	return 0;
}
