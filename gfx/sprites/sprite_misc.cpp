/*
 * sprite_temperature.cpp
 *
 *  Created on: Feb 27, 2017
 *      Author: shapa
 */

#include "sprites.hpp"

static const uint8_t s_dot[] = {
	0xFF, 0xF0,
	0xFF, 0xF0,
	0xFF, 0xF0,
};

const Sprite Sprite_Dot(
	ColorDepth_4,
	4,
	sizeof(s_dot) / 2,
	sizeof(s_dot),
	s_dot
);
