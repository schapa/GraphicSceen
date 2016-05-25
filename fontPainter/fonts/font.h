// Header (.h) for generated fonts

#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct {
    size_t offset;
    uint8_t width;
    uint8_t heigth;
    int8_t left;
    int8_t top;
    uint8_t advance;
} fontLookupItem_t, *fontLookupItem_p;

typedef struct {
    const fontLookupItem_p lookup;
    const uint8_t *pixelData;
    const uint8_t tallestChar;
    const uint8_t size;
} fontItem_t, *fontItem_p;

