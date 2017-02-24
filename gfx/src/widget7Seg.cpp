/*
 * widget7Seg.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: shapa
 */

#include "widget7Seg.hpp"


SegmentDisplayWidget::SegmentDisplayWidget(Type type): type(type), value(0) {
	shape = new GfxSpriteShape();
}
SegmentDisplayWidget::~SegmentDisplayWidget() {
	delete shape;
}
