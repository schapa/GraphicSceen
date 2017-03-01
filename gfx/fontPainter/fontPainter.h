/*
 * fontPainter.h
 *
 *  Created on: May 18, 2016
 *      Author: shapa
 */

#ifndef FONTPAINTER_H_
#define FONTPAINTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "font.h"

typedef enum {
	FONT_DIGITAL_7SEGMENT,
	FONT_CENTURY_SCOOLBOOK,
	FONT_CREAMPUFF,
	FONT_COOLVETICA,
	FONT_LAST,
} FontType;

fontItem_p FontPainter_SizeLookup(FontType type, uint8_t size);
uint16_t FontPainter_GetTextWidth(fontItem_p font, const char *text);

#ifdef __cplusplus
}
#endif

#endif /* FONTPAINTER_H_ */
