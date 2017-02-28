/*
 * sprite_temperature.cpp
 *
 *  Created on: Feb 27, 2017
 *      Author: shapa
 */

#include "sprites.hpp"

static const uint8_t s_celsius[] = {
	0x00, 0xFF, 0xFF, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00,
	0x0F, 0xFF, 0xFF, 0xF0,  0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xF0, 0x0F, 0xFF,  0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0xFF,  0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0xFF,  0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xF0, 0x0F, 0xFF,  0x00, 0x00, 0x00, 0x00, 0x00,
	0x0F, 0xFF, 0xFF, 0xF0,  0x0F, 0xFF, 0xFF, 0xFF, 0x00,
	0x00, 0xFF, 0xFF, 0x00,  0xFF, 0xFF, 0xFF, 0xFF, 0xF0,
	0x00, 0x00, 0x00, 0x00,  0xFF, 0xF0, 0x00, 0xFF, 0xF0,
	0x00, 0x00, 0x00, 0x0F,  0xFF, 0x00, 0x00, 0x0F, 0xF0,
	0x00, 0x00, 0x00, 0x0F,  0xFF, 0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x0F,  0xF0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0F,  0xF0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0F,  0xF0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0F,  0xF0, 0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x0F,  0xFF, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0F,  0xFF, 0x00, 0x00, 0x0F, 0xF0,
	0x00, 0x00, 0x00, 0x00,  0xFF, 0xF0, 0x00, 0xFF, 0xF0,
	0x00, 0x00, 0x00, 0x00,  0xFF, 0xFF, 0xFF, 0xFF, 0xF0,
	0x00, 0x00, 0x00, 0x00,  0x0F, 0xFF, 0xFF, 0xFF, 0x00,
};

static const uint8_t s_fahrenheit[] = {
	0x00, 0xFF, 0xFF, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00,
	0x0F, 0xFF, 0xFF, 0xF0,  0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xF0, 0x0F, 0xFF,  0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0xFF,  0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0x00, 0x00, 0xFF,  0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xF0, 0x0F, 0xFF,  0x00, 0x00, 0x00, 0x00, 0x00,
	0x0F, 0xFF, 0xFF, 0xF0,  0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0xFF, 0xFF, 0x00,  0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00,  0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00,  0x00, 0xFF, 0xF0, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,  0x00, 0xFF, 0xF0, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00,  0x00, 0xFF, 0xF0, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,  0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00,  0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00,  0x00, 0xFF, 0xFF, 0xFF, 0xFF,

	0x00, 0x00, 0x00, 0x00,  0x00, 0xFF, 0xF0, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,  0x00, 0xFF, 0xF0, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,  0x00, 0xFF, 0xF0, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,  0x00, 0xFF, 0xF0, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,  0x00, 0xFF, 0xF0, 0x00, 0x00,
};


static const uint8_t s_tripA[] = {
	0xFF, 0xFF, 0xF0,  0xFF, 0xF0,  0xFF, 0xFF, 0xF0,
	0x0F, 0xFF, 0xF0,  0xFF, 0xF0,  0xFF, 0xFF, 0x00,
	0x00, 0xFF, 0xF0,  0xFF, 0xF0,  0xFF, 0xF0, 0x00,
	0x00, 0x0F, 0xF0,  0xFF, 0xF0,  0xFF, 0x00, 0x00,
};

static const uint8_t s_tripB[] = {
	0xFF, 0xFF, 0xF0,  0xFF, 0xF0,  0xFF, 0xFF, 0xF0,
	0x0F, 0xFF, 0xF0,  0xFF, 0xF0,  0xFF, 0xFF, 0x00,
	0x00, 0xFF, 0xF0,  0xFF, 0xF0,  0xFF, 0xF0, 0x00,
	0x00, 0x0F, 0xF0,  0xFF, 0xF0,  0xFF, 0x00, 0x00,
};

const Sprite Sprite_Celsius(
	ColorDepth_4,
	18,
	sizeof(s_celsius) / 9,
	sizeof(s_celsius),
	s_celsius
);

const Sprite Sprite_Fahrenheit(
	ColorDepth_4,
	18,
	sizeof(s_fahrenheit) / 9,
	sizeof(s_fahrenheit),
	s_fahrenheit
);

const Sprite Sprite_TripA(
	ColorDepth_4,
	16,
	sizeof(s_tripA) / 8,
	sizeof(s_tripA),
	s_tripA
);

const Sprite Sprite_TripB(
	ColorDepth_4,
	16,
	sizeof(s_tripB) / 8,
	sizeof(s_tripB),
	s_tripB
);
