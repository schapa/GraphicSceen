/*
 * surface.cpp
 *
 *  Created on: May 24, 2016
 *      Author: shapa
 */

#include "surface.hpp"
#include <string.h>


GfxSurface::GfxSurface(void) {
	line = NULL;
	bytesPerLine = 0;
	width = 0;
	heigth = 0;
	bitsDepth = ColorDepth_4;
	pixelFormat = PixelFormat_GrayScale;
	bytesPerPixel = 0;
}

GfxSurface::GfxSurface(ColorDepth bitsDepth, uint16_t width, uint16_t heigth) : GfxSurface() {
	this->bitsDepth = bitsDepth;
	this->pixelFormat = PixelFormat_GrayScale;
	this->width = width;
	this->heigth = heigth;
	create();
}

GfxSurface::GfxSurface(PixelFormat pixFormat, uint16_t width, uint16_t heigth) {
	this->pixelFormat = pixFormat;
	this->bitsDepth = format2depth(pixFormat);
	this->width = width;
	this->heigth = heigth;
	create();
}

GfxSurface::~GfxSurface(void) {
	if (line) {
		line = NULL;
	}
}

void GfxSurface::fill(uint32_t val) {
	switch (bitsDepth) {
		case ColorDepth_4:
			val &= 0x0F;
			val |= val<<4;
			memset(line, val, heigth*bytesPerLine);
			break;
		case ColorDepth_8:
			memset(line, val, heigth*bytesPerLine);
			break;
		case ColorDepth_16: {
			for (size_t i = 0; i < heigth; i++) {
				for (size_t j = 0; j < width; j++) {
					uint16_t *ptr = reinterpret_cast<uint16_t *>(&line[i*bytesPerLine + j*bytesPerPixel]);
					*ptr = val;
				}
			}
			break;
		}
		case ColorDepth_24: {
			int *ptr = NULL;
			*ptr = 1; // stop here;
			break;
		}
		case ColorDepth_32: {
			uint32_t **ptr = reinterpret_cast<uint32_t **>(line);
			for (size_t i = 0; i < heigth; i++) {
				for (size_t j = 0; j < width; j++)
					ptr[i][j] = val;
			}
			break;
		}
		default:
			break;
	}
}

void GfxSurface::drawPixel(uint16_t x, uint16_t y, uint8_t alpha) {
	if (x >= width || y >= heigth)
		return;

	const size_t offset = (bitsDepth == ColorDepth_4) ?
			(y*bytesPerLine + x/2) : (y*bytesPerLine + x*bytesPerPixel);
	uint8_t *ptrRaw = &line[offset];

	switch (bitsDepth) {
		case ColorDepth_4: {
			alpha &= 0x0F;
			if (x % 2) {
				*ptrRaw &= 0xF0;
				*ptrRaw |= alpha;
			} else {
				*ptrRaw &= 0x0F;
				*ptrRaw |= alpha<<4;
			}
			break;
		}
		default:
			switch (pixelFormat) {
			case PixelFormat_GrayScale:
				// should newer get here
				break;
			case PixelFormat_ARGB8888: {
				uint32_t pix = alpha<<24 | 0xFFFFFF;
				uint32_t *ptr = reinterpret_cast<uint32_t *>(ptrRaw);
				*ptr = pix;
				break;
			}
			case PixelFormat_RGB888: {
				uint8_t *ptr = &line[offset];
				ptr[0] = alpha;
				ptr[1] = alpha;
				ptr[2] = alpha;
				break;
			}
			case PixelFormat_RGB565: {
				uint16_t pix = alpha>>3;
				pix |= pix<<11;
				pix |= (alpha>>2)<<5;
				uint16_t *ptr = reinterpret_cast<uint16_t *>(ptrRaw);
				*ptr = pix;
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

void GfxSurface::drawPixel(uint16_t x, uint16_t y, uint32_t argb) {
	if (x >= width || y >= heigth)
		return;

	const size_t offset = (bitsDepth == ColorDepth_4) ?
			(y*bytesPerLine + x/2) : (y*bytesPerLine + x*bytesPerPixel);
	uint8_t *ptrRaw = &line[offset];

	switch (bitsDepth) {
		case ColorDepth_4: {
			argb &= 0x0F;
			if (x % 2) {
				*ptrRaw &= 0xF0;
				*ptrRaw |= argb;
			} else {
				*ptrRaw &= 0x0F;
				*ptrRaw |= argb<<4;
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
				uint8_t *ptr = &line[offset];
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

uint32_t GfxSurface::getPixel(uint16_t x, uint16_t y) {
	if (x >= width || y >= heigth)
		return 0;

	const size_t offset = (bitsDepth == ColorDepth_4) ?
			(y*bytesPerLine + x/2) : (y*bytesPerLine + x*bytesPerPixel);
	uint8_t *ptrRaw = &line[offset];

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
}

void GfxSurface::create(void) {
	if (bitsDepth > 32) {
//		assert() here
		return;
	}
	bytesPerPixel = ColorDepth_4/8;

	bytesPerLine = (bitsDepth == ColorDepth_4) ?
			width/2 + width%2 : width*bytesPerPixel /*+ width%(bitsDepth/8)*/;
	line = (uint8_t*)malloc(heigth * bytesPerLine);
	if (line) {
		fill(0);
	}
}

ColorDepth GfxSurface::format2depth(const PixelFormat &fmt) {
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
