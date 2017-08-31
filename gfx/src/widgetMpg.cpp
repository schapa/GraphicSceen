/*
 * widgetMpg.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: shapa
 */

#include "widgetMpg.hpp"
#include "sprites.hpp"

#include <assert.h>

#include "dbg_base.h"
#if 0
#include "dbg_trace.h"
#endif

class Icons : public GfxSpriteShape {
public:
	Icons() {
		SpriteItem *arrow = new SpriteItem(0, 9, Sprite_HalfArrow);
		SpriteItem *gas = new SpriteItem(10, 0, Sprite_GasStation);
		sprites.push_back(arrow);
		sprites.push_back(gas);

		arrow->isVisible = true;
		gas->isVisible = true;
	};
	virtual ~Icons() {};
};

MPGWidget::MPGWidget(const Type& type): type(type) {
	const uint16_t line2_x = 6;
	const uint16_t line2_y = 20;

	avg = new TextWidget(FONT_LIBEL_SUIT, 16, "AVG.");
	shapes.push_back(avg);

	mpg = new TextWidget(FONT_LIBEL_SUIT, 16, "MPG");
	mpg->setX(23);
	mpg->setY(0);
	shapes.push_back(mpg);

	liter = new TextWidget(FONT_LIBEL_SUIT, 16, "l");
	liter->setX(19);
	liter->setY(3);
	shapes.push_back(liter);

	line = new GfxLineShape();
	line->setX(line2_x);
	line->setY(20);
	line->drawLine(0,0, 32,0);
	line->setVisible(true);
	shapes.push_back(line);

	icons = new Icons();
	icons->setX(line2_x + 34);
	icons->setY(8);
	icons->setVisible(true);
	shapes.push_back(icons);

	hunred = new TextWidget(FONT_LIBEL_SUIT, 16, "100");
	hunred->setX(line2_x);
	hunred->setY(line2_y);
	shapes.push_back(hunred);

	k = new TextWidget(FONT_LIBEL_SUIT, 16, "k");
	k->setX(line2_x + 16);
	k->setY(line2_y);
	shapes.push_back(k);

	m = new TextWidget(FONT_LIBEL_SUIT, 16, "m");
	m->setX(line2_x + 22);
	m->setY(line2_y);
	shapes.push_back(m);

	i = new TextWidget(FONT_LIBEL_SUIT, 16, "i");
	i->setX(line2_x + 32);
	i->setY(line2_y);
	shapes.push_back(i);

	perLiter = new TextWidget(FONT_LIBEL_SUIT, 16, "/l");
	perLiter->setX(line2_x + 35);
	perLiter->setY(line2_y);
	shapes.push_back(perLiter);

	for (auto it = shapes.begin(); it != shapes.end(); it++)
		(*it)->createSurface();
	update();
}


void MPGWidget::update() {
	for (auto it = shapes.begin(); it != shapes.end(); it++)
		(*it)->setVisible(false);
	icons->setVisible(true);
	line->setVisible(true);
	switch (type) {
		case EDM:
			liter->setVisible(true);
			line->setVisible(true);
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
