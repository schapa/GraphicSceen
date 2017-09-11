/*
 * SubiClcokScreen.cpp
 *
 *  Created on: Jun 16, 2017
 *      Author: shapa
 */

#include "SubiClockScreen.hpp"

#include "ssd1322.h"

extern "C" void DiscoLCDInit(uint8_t *);

void SubiClcokScreen::init() {
	temperature->setVisible(true);
	temperature->setX(0);
	temperature->setY(14);
	temperature->setTemperature(0);

	trip->setVisible(true);
	trip->setX(62);
	trip->setY(14);
	trip->setValue(0);

	mpg->setVisible(true);
	mpg->setX(132);
	mpg->setY(10);

	clock->setVisible(true);
	clock->setX(194);
	clock->setY(14);
	clock->setTime(12, 00);

	baseLayer->addShape(temperature);
	baseLayer->addShape(trip);
	baseLayer->addShape(mpg);
	baseLayer->addShape(clock);

#if !defined(EMULATOR) && 01
	if (!isOled)
		DiscoLCDInit(baseLayer->getFrameBuffer());
#endif
}

void SubiClcokScreen::draw() {
	baseLayer->render();
	if (isOled)
		SSD1322_DrawSurface(baseLayer->getFrameBuffer(), baseLayer->getHeight(), baseLayer->getBytesPerLine());
}
