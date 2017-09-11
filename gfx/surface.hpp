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
	explicit GfxSurface(void);
public:
	explicit GfxSurface(ColorDepth bitsDepth, uint16_t width, uint16_t height, const bool& create = true);
	explicit GfxSurface(PixelFormat pixFormat, uint16_t width, uint16_t height, const bool& create = true);
	virtual ~GfxSurface(void);
	void fill(uint32_t value);
public:
	void setFrameBuffer(uint8_t *fb);
	uint8_t *getFrameBuffer(void) const { return fb; }
	const uint16_t &getBytesPerLine(void) const { return bytesPerLine; }
	const uint16_t &getWidth(void) const { return width; }
	const uint16_t &getHeight(void) const { return height; }
	const ColorDepth &getBitsDepth(void) const { return bitsDepth; }
	const PixelFormat &getPixelFormat(void) const { return pixelFormat; }
	const uint32_t getPixel(const uint16_t &x, const uint16_t &y) const ;

	virtual void drawPixel(const uint16_t &x, const uint16_t &y, const uint32_t &argb, const PixelFormat &src);

private:
	void create(const bool &creat);
	const ColorDepth format2depth(const PixelFormat &fmt) const;
	const inline uint32_t convertPixel(const PixelFormat &src, const uint32_t &pix) const;
private:
	uint8_t *fb;
	uint16_t bytesPerLine;
	uint16_t bytesPerPixel;
	uint16_t width;
	uint16_t height;
	ColorDepth bitsDepth;
	PixelFormat pixelFormat;
};



#endif /* SURFACE_HPP_ */
