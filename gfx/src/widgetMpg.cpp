/*
 * widgetMpg.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: shapa
 */

#include "widgetMpg.hpp"

#include <assert.h>

#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif

MPGWidget::MPGWidget(const Type& type): type(type) {
	const uint16_t line2_x = 6;
	const uint16_t line2_y = 20;
	avg = new TextWidget(FONT_LIBEL_SUIT, 16, "AVG.");
	mpg = new TextWidget(FONT_LIBEL_SUIT, 16, "MPG");
	liter = new TextWidget(FONT_LIBEL_SUIT, 16, "l");
	line = new GfxLineShape();
	hunred = new TextWidget(FONT_LIBEL_SUIT, 16, "100");
	k = new TextWidget(FONT_LIBEL_SUIT, 16, "k");
	m = new TextWidget(FONT_LIBEL_SUIT, 16, "m");
	i = new TextWidget(FONT_LIBEL_SUIT, 16, "i");
	perLiter = new TextWidget(FONT_LIBEL_SUIT, 16, "/l");

	mpg->setX(23);
	mpg->setY(0);

	liter->setX(19);
	liter->setY(3);


	line->setX(line2_x);
	line->setY(20);
	line->drawLine(0,0, 32,0);
	line->setVisible(true);

	hunred->setX(line2_x);
	hunred->setY(line2_y);

	k->setX(line2_x + 16);
	k->setY(line2_y);

	m->setX(line2_x + 22);
	m->setY(line2_y);

	i->setX(line2_x + 32);
	i->setY(line2_y);

	perLiter->setX(line2_x + 35);
	perLiter->setY(line2_y);

	shapes.push_back(avg);
	shapes.push_back(mpg);
	shapes.push_back(liter);
	shapes.push_back(line);
	shapes.push_back(hunred);
	shapes.push_back(k);
	shapes.push_back(m);
	shapes.push_back(i);
	shapes.push_back(perLiter);
	for (size_t i = 0; i < shapes.size(); i++)
		shapes[i]->createSurface();
	update();
}


void MPGWidget::update() {
	for (size_t i = 0; i < shapes.size(); i++)
		shapes[i]->setVisible(false);
	switch (type) {
		case EDM:
			liter->setVisible(true);
			hunred->setVisible(true);
			k->setVisible(true);
			m->setVisible(true);
			break;
		case USDM:
			avg->setVisible(true);
			mpg->setVisible(true);
			break;
		case JDM:
			k->setVisible(true);
			m->setVisible(true);
			perLiter->setVisible(true);
			break;
	}
	dirty = true;
}
