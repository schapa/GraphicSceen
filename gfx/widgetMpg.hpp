/*
 * widgetMpg.hpp
 *
 *  Created on: Mar 1, 2017
 *      Author: shapa
 */

#pragma once

#include "shapeMulti.hpp"
#include "widgetText.hpp"

class MPGWidget: public GfxMultiShape {
public:
	enum Type {
		EDM,
		USDM,
		JDM
	};
public:
	MPGWidget(const Type& type = EDM);
	virtual ~MPGWidget() {};

	void setType(const Type& type) { this->type = type; update(); }
private:
	Type type;
	void update();
private:
	TextWidget *avg;
	TextWidget *mpg;
	TextWidget *liter;
	TextWidget *hunred;
	TextWidget *k;
	TextWidget *m;
	TextWidget *i;
	TextWidget *perLiter;
};
