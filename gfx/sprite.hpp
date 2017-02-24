/*
 * sprite.hpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include "gfx.hpp"

class Sprite {
public:
	Sprite(ColorDepth depth, uint16_t width, uint16_t height, size_t size, const void *const data):
		depth(depth), width(width), height(height), size(size), data(data) {};

	const uint16_t& getWidth() const { return width; }
	const uint16_t& getHeight() const { return height; }
	const size_t& getSize() const { return size; }
	const ColorDepth& getDepth() const { return depth; }

	const uint32_t getPixel(const uint16_t& x, const uint16_t& y) const {
		assert(x < width);
		assert(y < height);
		switch (depth) {
			case ColorDepth_4: {
				const uint8_t*val = static_cast<const uint8_t*>(data);
				return (y % 2) ?
						val[x*width + y/2] & 0xF0 :
						val[x*width +y/2] >> 0x0F;
				break;
			}
			case ColorDepth_8: {
				const uint8_t*val = static_cast<const uint8_t*>(data);
				return val[x*width +y];
				break;
			}
			case ColorDepth_16: {
				const uint16_t*val = static_cast<const uint16_t*>(data);
				return val[x*width +y];
				break;
			}
			case ColorDepth_24: {
				assert(!"Implement me");
	//				const uint8_t*val = static_cast<const uint8_t*>(data);
	//				return val[x*width + y/3];
				break;
			}
			case ColorDepth_32: {
				const uint32_t*val = static_cast<const uint32_t*>(data);
				return val[x*width +y];
				break;
			}
		}
		return 0;
	}
private:
	const ColorDepth depth;
	const uint16_t width;
	const uint16_t height;
	const size_t size;
	const void *const data;
};

class SpriteItem {

public:
	bool isVisible;
	SpriteItem(const uint16_t& x, const uint16_t& y, const Sprite& sprite): isVisible(false), x(x), y(y), sprite(sprite) {};

public:
	const uint16_t& getX() const { return x; }
	const uint16_t& getY() const { return y; }
	void setX(uint16_t& x) { this->x = x; }
	void setY(uint16_t& y) { this->y = y;  }
	const Sprite& getSprite() const { return sprite; }

private:
	uint16_t x;
	uint16_t y;
	const Sprite& sprite;
};

