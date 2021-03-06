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
	explicit GfxSurface();
public:
	explicit GfxSurface(ColorDepth bitsDepth, uint16_t width, uint16_t height, const bool& create = true);
	explicit GfxSurface(PixelFormat pixFormat, uint16_t width, uint16_t height, const bool& create = true);
	virtual ~GfxSurface();
	void fill(uint32_t value);
public:
	void setFrameBuffer(uint8_t *fb, bool isHeap);
	uint8_t *getFrameBuffer() const { return fb; }
	size_t getFrameBufferSize() const { return height * bytesPerLine; }
	const uint16_t &getBytesPerLine() const { return bytesPerLine; }
	const uint16_t &getWidth() const { return width; }
	const uint16_t &getHeight() const { return height; }
	const ColorDepth &getBitsDepth() const { return bitsDepth; }
	const PixelFormat &getPixelFormat() const { return pixelFormat; }
	const uint32_t getPixel(const uint16_t &x, const uint16_t &y) const ;

	virtual void drawPixel(const uint16_t &x, const uint16_t &y, const uint32_t &argb, const PixelFormat &src);

private:
	void create(const bool &creat);
	const ColorDepth format2depth(const PixelFormat &fmt) const;
	const inline uint32_t convertPixel(const PixelFormat &src, const uint32_t &pix) const;
private:
	uint8_t *fb;
	bool heapFb;
	uint16_t bytesPerLine;
	uint16_t bytesPerPixel;
	uint16_t width;
	uint16_t height;
	ColorDepth bitsDepth;
	PixelFormat pixelFormat;
};



#endif /* SURFACE_HPP_ */
