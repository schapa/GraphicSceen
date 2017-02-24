/*
 * gfx.h
 *
 *  Created on: May 17, 2016
 *      Author: shapa
 */

#pragma once

enum ColorDepth {
	ColorDepth_4 = 4,
	ColorDepth_8 = 8,
	ColorDepth_16 = 16,
	ColorDepth_24 = 24,
	ColorDepth_32 = 32,
};

enum PixelFormat {
	PixelFormat_GrayScale,
	PixelFormat_ARGB8888,
	PixelFormat_RGB888,
	PixelFormat_RGB565,
	PixelFormat_ARGB1555,
	PixelFormat_ARGB4444,
	PixelFormat_L8,
	PixelFormat_AL44,
	PixelFormat_AL88,
};

//class Pixel {
//public:
//	Pixel();
//	virtual ~Pixel();
//
//	const ColorDepth& getDepth() const { return depth; }
//	const PixelFormat& getformat() const { return format; }
//private:
//	uint32_t value;
//
//	ColorDepth depth;
//	PixelFormat format;
//};
