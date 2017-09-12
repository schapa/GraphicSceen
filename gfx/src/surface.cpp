/*
 * surface.cpp
 *
 *  Created on: May 24, 2016
 *      Author: shapa
 */

#include "surface.hpp"
#include "memman.h"
#include <string.h>
#include <assert.h>


GfxSurface::GfxSurface() :
	fb(NULL),
	bytesPerLine(0),
	bytesPerPixel(0),
	width(0),
	height(0),
	bitsDepth(ColorDepth_4),
	pixelFormat(PixelFormat_GrayScale) {}

GfxSurface::GfxSurface(ColorDepth bitsDepth, uint16_t width, uint16_t height, const bool& creat) : GfxSurface() {
	this->width = width;
	this->height = height;
	this->pixelFormat = PixelFormat_GrayScale;
	this->bitsDepth = bitsDepth;
	create(creat);
}

GfxSurface::GfxSurface(PixelFormat pixFormat, uint16_t width, uint16_t height, const bool& creat) : GfxSurface() {
	this->width = width;
	this->height = height;
	this->pixelFormat = pixFormat;
	this->bitsDepth = format2depth(pixFormat);
	create(creat);
}

GfxSurface::~GfxSurface() {
	MEMMAN_free(fb);
	fb = NULL;
}

void GfxSurface::fill(uint32_t val) {
	assert(fb);
	switch (bitsDepth) {
		case ColorDepth_4:
			val &= 0x0F;
			val |= val<<4;
			memset(fb, val, height*bytesPerLine);
			break;
		case ColorDepth_8:
			memset(fb, val, height*bytesPerLine);
			break;
		case ColorDepth_16: {
			for (size_t i = 0; i < height; i++) {
				for (size_t j = 0; j < width; j++) {
					uint16_t *ptr = reinterpret_cast<uint16_t *>(&fb[i*bytesPerLine + j*bytesPerPixel]);
					*ptr = val;
				}
			}
			break;
		}
		case ColorDepth_24: {
			assert(!"couldnt handle");
			break;
		}
		case ColorDepth_32: {
			for (size_t i = 0; i < height; i++) {
				for (size_t j = 0; j < width; j++) {
					uint32_t *ptr = reinterpret_cast<uint32_t *>(&fb[i*bytesPerLine + j*bytesPerPixel]);
					*ptr = val;
				}
			}
			break;
		}
		default:
			break;
	}
}

//void GfxSurface::drawPixel(const uint16_t &x, const uint16_t &y, const uint8_t &alpha) {
//	if (x >= width || y >= height)
//		return;
//
//	const size_t offset = (bitsDepth == ColorDepth_4) ?
//			(y*bytesPerLine + x/2) : (y*bytesPerLine + x*bytesPerPixel);
//	uint8_t *ptrRaw = &line[offset];
//
//	switch (bitsDepth) {
//		case ColorDepth_4: {
//			const uint8_t tmp = alpha/16;
//			if (x % 2) {
//				*ptrRaw &= 0xF0;
//				*ptrRaw |= tmp;
//			} else {
//				*ptrRaw &= 0x0F;
//				*ptrRaw |= tmp<<4;
//			}
//			break;
//		}
//		default:
//			switch (pixelFormat) {
//			case PixelFormat_GrayScale:
//				// should newer get here
//				break;
//			case PixelFormat_ARGB8888: {
//				uint32_t pix = alpha<<24 | 0xFFFFFF;
//				uint32_t *ptr = reinterpret_cast<uint32_t *>(ptrRaw);
//				*ptr = pix;
//				break;
//			}
//			case PixelFormat_RGB888: {
//				uint8_t *ptr = &line[offset];
//				ptr[0] = alpha;
//				ptr[1] = alpha;
//				ptr[2] = alpha;
//				break;
//			}
//			case PixelFormat_RGB565: {
//				uint16_t pix = alpha>>3;
//				pix |= pix<<11;
//				pix |= (alpha>>2)<<5;
//				uint16_t *ptr = reinterpret_cast<uint16_t *>(ptrRaw);
//				*ptr = pix;
//				break;
//			}
//			case PixelFormat_ARGB1555:
//			case PixelFormat_ARGB4444:
//			case PixelFormat_AL88:
//				break;
//			case PixelFormat_L8:
//			case PixelFormat_AL44:
//				break;
//			}
//	}
//}

void GfxSurface::drawPixel(const uint16_t &x, const uint16_t &y, const uint32_t &argbInp, const PixelFormat &src) {
	assert(fb);
	if (x >= width || y >= height)
		return;
	const uint32_t &argb = convertPixel(src, argbInp);
	const size_t offset = (bitsDepth == ColorDepth_4) ?
			(y*bytesPerLine + x/2) : (y*bytesPerLine + x*bytesPerPixel);
	uint8_t *ptrRaw = &fb[offset];

	switch (bitsDepth) {
		case ColorDepth_4: {
			const uint32_t tmp = argb & 0x0F;
			if (x % 2) {
				*ptrRaw &= 0xF0;
				*ptrRaw |= tmp;
			} else {
				*ptrRaw &= 0x0F;
				*ptrRaw |= tmp<<4;
			}
			break;
		}
		default:
			switch (pixelFormat) {
			case PixelFormat_GrayScale:
				// should newer get here
				break;
			case PixelFormat_ARGB8888: {
				uint32_t *ptr = reinterpret_cast<uint32_t *>(ptrRaw);
				*ptr = argb;
				break;
			}
			case PixelFormat_RGB888: {
				uint8_t *ptr = &fb[offset];
				ptr[0] = argb & 0xFF;
				ptr[1] = argb>>8 & 0xFF;;
				ptr[2] = argb>>16 & 0xFF;;
				break;
			}
			case PixelFormat_RGB565: {
				uint16_t *ptr = reinterpret_cast<uint16_t *>(ptrRaw);
				*ptr = argb & 0xFFFF;
				break;
			}
			case PixelFormat_ARGB1555:
			case PixelFormat_ARGB4444:
			case PixelFormat_AL88:
				break;
			case PixelFormat_L8:
			case PixelFormat_AL44:
				break;
			}
	}
}

const uint32_t GfxSurface::getPixel(const uint16_t &x, const uint16_t &y) const {
	assert(fb);
	if (x >= width || y >= height)
		return 0;

	const size_t offset = (bitsDepth == ColorDepth_4) ?
			(y*bytesPerLine + x/2) : (y*bytesPerLine + x*bytesPerPixel);
	uint8_t *ptrRaw = &fb[offset];

	switch (bitsDepth) {
	case ColorDepth_4:
		return (x % 2) ? *ptrRaw & 0x0F : *ptrRaw>>4;
	case ColorDepth_8:
		return *ptrRaw;
	case ColorDepth_16:
		return *reinterpret_cast<uint16_t *>(ptrRaw);
	case ColorDepth_32:
		return *reinterpret_cast<uint32_t *>(ptrRaw);
	default:
		return 0;
	}
	return 0;
}

void GfxSurface::setFrameBuffer(uint8_t *fb) {
	assert(fb);
	MEMMAN_free(this->fb);
	this->fb = fb;
}

void GfxSurface::create(const bool &creat) {
	assert (bitsDepth <= 32);
	MEMMAN_free(fb);
	fb = NULL;
	bytesPerPixel = bitsDepth/8;

	bytesPerLine = (bitsDepth == ColorDepth_4) ?
			width/2 + width%2 : width*bytesPerPixel /*+ width%(bitsDepth/8)*/;

	if (!creat)
		return;

	fb = (uint8_t*)MEMMAN_malloc(height * bytesPerLine);
	assert (fb);
	if (fb) {
		fill(0);
	}
}

const ColorDepth GfxSurface::format2depth(const PixelFormat &fmt) const {
	ColorDepth depth = ColorDepth_32;
	switch (fmt) {
		case PixelFormat_GrayScale:
			depth = ColorDepth_4; //TODO:
			break;
		case PixelFormat_ARGB8888:
			depth = ColorDepth_32;
			break;
		case PixelFormat_RGB888:
			depth = ColorDepth_24;
			break;
		case PixelFormat_RGB565:
		case PixelFormat_ARGB1555:
		case PixelFormat_ARGB4444:
		case PixelFormat_AL88:
			depth = ColorDepth_16;
			break;
		case PixelFormat_L8:
		case PixelFormat_AL44:
			depth = ColorDepth_8;
			break;
	}
	return depth;
}

const uint32_t GfxSurface::convertPixel(const PixelFormat &src, const uint32_t &pix) const {
	if (pixelFormat == src)
		return pix;
	uint32_t newVal = 0;
	switch (pixelFormat) {
		case PixelFormat_GrayScale:
			switch (src) {
				case PixelFormat_RGB565: {
					const uint8_t b = pix & 0x1F;
					const uint8_t g = (pix>>6) & 0x3F;
					const uint8_t r = (pix>>11) & 0x1F;
					newVal = (b + g + r)/192;
					break;
				}
				case PixelFormat_L8: {
					newVal = pix/16;
					break;
				}
				default:
					break;
			}
			break;
		case PixelFormat_RGB565:
			if (src == PixelFormat_GrayScale) {
				newVal = pix<<1;
				newVal |= pix<<7;
				newVal |= pix<<12;
			}
			break;
		default:
			newVal = pix;
			break;
	}
	return newVal;
}
