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

	const uint32_t getPixel(const uint16_t& x, const uint16_t& y) const;
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
	const Sprite& sprite;
	SpriteItem(const uint16_t& x, const uint16_t& y, const Sprite& sprite): isVisible(true), sprite(sprite), x(x), y(y) {};

public:
	const uint16_t& getX() const { return x; }
	const uint16_t& getY() const { return y; }
	void setX(uint16_t& x) { this->x = x; }
	void setY(uint16_t& y) { this->y = y;  }

private:
	uint16_t x;
	uint16_t y;
};

