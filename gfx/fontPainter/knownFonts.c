/*
 * knownFonts.c
 *
 *  Created on: May 18, 2016
 *      Author: shapa
 */

#include <stdint.h>

#include "fontPainter.h"
//#include "century_schoolbook_l_bold_8_font.h"
#include "century_schoolbook_l_bold_12_font.h"
#include "digital_7_regular_12_font.h"
#include "digital_7_regular_18_font.h"
#include "coolvetica_regular_12_font.h"
#include "libel_suit_regular_16_font.h"


typedef struct {
	const fontItem_p *fonts;
	const size_t count;
} fontSizeMap_t, *fontSizeMap_p;


static const fontItem_p s_digital_7_regular_font[] = {
		(const fontItem_p)&digital_7_regular_12_font,
		(const fontItem_p)&digital_7_regular_18_font,
};
static const fontItem_p s_century_schoolbook_bold_font[] = {
		(const fontItem_p)&century_schoolbook_l_bold_12_font,
};
static const fontItem_p s_creampuff_regular_font[] = {
};

static const fontItem_p s_coolvetica_regular_font[] = {
		(const fontItem_p)&coolvetica_regular_12_font,
};
static const fontItem_p libel_suit_regular_font[] = {
		(const fontItem_p)&libel_suit_regular_16_font,
};


static const fontSizeMap_t s_knownFonts[] = {
	[FONT_DIGITAL_7SEGMENT] = { s_digital_7_regular_font, sizeof(s_digital_7_regular_font)/sizeof(fontItem_p) },
	[FONT_CENTURY_SCOOLBOOK] = { s_century_schoolbook_bold_font, sizeof(s_century_schoolbook_bold_font)/sizeof(fontItem_p) },
	[FONT_CREAMPUFF] = { s_creampuff_regular_font, sizeof(s_creampuff_regular_font)/sizeof(fontItem_p) },
	[FONT_COOLVETICA] = { s_coolvetica_regular_font, sizeof(s_coolvetica_regular_font)/sizeof(fontItem_p) },
	[FONT_LIBEL_SUIT] = { libel_suit_regular_font, sizeof(libel_suit_regular_font)/sizeof(fontItem_p) },
};


fontItem_p FontPainter_SizeLookup(FontType type, uint8_t size) {
	if (type < FONT_LAST) {
		size_t i;
		for (i = 0; i < s_knownFonts[type].count; i++) {
			fontItem_p font = s_knownFonts[type].fonts[i];
			if (font && font->size == size)
				return font;
		}
	}
	return NULL;
}


uint16_t FontPainter_GetTextWidth(fontItem_p font, const char *text) {
	uint16_t width = 0;
	if (!font || !text)
		return width;
	fontLookupItem_p lookup = font->lookup;
	while (*text) {
		width += lookup[(size_t)*text++].advance;
	}
	return width;
}
