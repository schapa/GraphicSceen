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
}

GfxSurface::GfxSurface(ColorDepth bitsDepth, uint16_t width, uint16_t heigth) : GfxSurface() {
	this->bitsDepth = bitsDepth;
	this->width = width;
	this->heigth = heigth;
	create();
}

GfxSurface::~GfxSurface(void) {
	if (line) {
		for (size_t i = 0; i < heigth; i++) {
			free(line[i]);
		}
		free(line);
	}
}

void GfxSurface::fill(uint32_t val) {
	switch (bitsDepth) {
		case 4:
			val &= 0x0F;
			val |= val<<4;
			for (size_t i = 0; i < heigth; i++) {
				memset(line[i], val, bytesPerLine);
			}
			break;
		case 8:
			for (size_t i = 0; i < heigth; i++) {
				memset(line[i], val, bytesPerLine);
			}
			break;
		default:
			break;
	}
}

void GfxSurface::drawPixel(uint16_t x, uint16_t y, uint8_t brightness) {
	if (x >= width || y >= heigth)
		return;

	brightness &= 0x0F;
	if (x % 2) {
		line[y][x/2] &= 0xF0;
		line[y][x/2] |= brightness;
	} else {
		line[y][x/2] &= 0x0F;
		line[y][x/2] |= brightness<<4;
	}
}

uint32_t GfxSurface::getPixel(uint16_t x, uint16_t y) {
	if (x >= width || y >= heigth)
		return 0;
	switch (bitsDepth) {
	case ColorDepth_4:
		return (x % 2) ? line[y][x/2] & 0x0F : line[y][x/2]>>4;
	default:
		return 0;
	}
}

void GfxSurface::create(void) {
	if (bitsDepth > 32) {
//		assert() here
		return;
	}
	bytesPerLine = width*bitsDepth/8 /*+ width%(bitsDepth/8)*/;
	line = (uint8_t**)malloc(heigth * sizeof(*line));
	if (line) {
		memset(line, 0, heigth * sizeof(*line));
		for (size_t i = 0; i < heigth; i++) {
			line[i] = (uint8_t*)malloc(bytesPerLine);
			if (!line[i]) {
				do {
					free(line[i]);
					line[i] = NULL;
				} while (i--);
				free(line);
				line = NULL;
				return;
			}
		}
	}
	fill(0);
}
