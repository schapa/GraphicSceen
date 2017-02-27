/*
 * surface.hpp
 *
 *  Created on: May 24, 2016
 *      Author: shapa
 */

#ifndef SURFACE_HPP_
#define SURFACE_HPP_

#include <stdlib.h>
#include <stdint.h>

#include "gfx.hpp"

class GfxSurface {
private:
	GfxSurface(void);
public:
	GfxSurface(ColorDepth bitsDepth, uint16_t width, uint16_t heigth);
	GfxSurface(PixelFormat pixFormat, uint16_t width, uint16_t heigth);
	virtual ~GfxSurface(void);
	void fill(uint32_t value);
public:
	uint8_t *getFrameBuffer(void) { return line; }
	const uint16_t &getBytesPerLine(void) { return bytesPerLine; }
	const uint16_t &getWidth(void) { return width; }
	const uint16_t &getHeigth(void) { return heigth; }
	const ColorDepth &getBitsDepth(void) { return bitsDepth; }
	const PixelFormat &getPixelFormat(void) { return pixelFormat; }
public:
	virtual void drawPixel(const uint16_t &x, const uint16_t &y, const uint8_t &alpha);
	virtual void drawPixel(const uint16_t &x, const uint16_t &y, const uint32_t &argb);

	const uint32_t getPixel(const uint16_t &x, const uint16_t &y);
private:
	void create(void);
	ColorDepth format2depth(const PixelFormat &fmt);
private:
	uint8_t *line;
	uint16_t bytesPerLine;
	uint16_t bytesPerPixel;
	uint16_t width;
	uint16_t heigth;
	ColorDepth bitsDepth;
	PixelFormat pixelFormat;
};



#endif /* SURFACE_HPP_ */
