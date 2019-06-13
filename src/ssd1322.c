/*
 * ssd1322.c
 *
 *  Created on: May 20, 2016
 *      Author: shapa
 */

#include "ssd1322.h"
#include "bsp.h"
#include "system.h"

#define COLUMN_MAX 119
#define ROW_MAX 127

static void (*Delay)(uint32_t delay) = System_delayMsDummy;
static void (*Reset)(_Bool state) = BSP_LcdReset;
static void (*CS)(_Bool state) = BSP_LcdCs;
static void (*CMD)(uint8_t val) = BSP_LcdCmd;
static void (*Data)(const uint8_t *buff, uint16_t size) = BSP_LcdWrite;

static void UpdateGrayScaleTable(void);

static void SendCmd_Simple(uint8_t cmd);
static void SendCmd_One(uint8_t cmd, uint8_t val);
static void SendCmd_Two(uint8_t cmd, uint8_t valA, uint8_t valB);

void SSD1322_InitDisplay(void) {
	Reset(true);
	Delay(2);
	Reset(false);
	Delay(2);

	SSD1322_SetCmdLock(false);
	SendCmd_Simple(SSD1322_SET_SLEEP_OFF);
	SSD1322_SetColumnRange(0x00, COLUMN_MAX);
	SSD1322_SetRowRange(0x00, 0x3F);
	SendCmd_One(SSD1322_SET_MUX_RATIO, 0x3F);
	SendCmd_Two(SSD1322_SET_REMAP_COM_LINE_MODE, 0x14, 0x11);
	SendCmd_Simple(SSD1322_SET_DEFAULT_GRAY_SCALE_TABLE);
	SendCmd_Simple(SSD1322_ENABLE_GRAYSCALE_TABLE);
	SendCmd_One(SSD1322_SET_PHASE_LENGTH, 0x32);
	SendCmd_One(SSD1322_SET_CLOCK_DIVIDER, 0xF0);
	SendCmd_Two(SSD1322_SET_ENHANCEMENT_A, 0xA0, 0xFD);

	SendCmd_One(SSD1322_SET_PRECHARGE_VOLTAGE, 0x1F);
	SendCmd_One(SSD1322_SET_PRECHARGE_PERIOD_2, 0x00);
	SendCmd_One(SSD1322_SET_VCOMM_VOLTAGE, 0x07);

	SendCmd_One(SSD1322_SET_DISPLAY_OFFSET, 0x00);
	SendCmd_One(SSD1322_SET_DISPLAY_START_LINE, 0x00);

	SendCmd_One(SSD1322_SET_CONTRAST_CURRENT, 0xFF);
	SendCmd_One(SSD1322_SET_MASTER_CONTRAST_CURRENT, 0x0F);

//	SendCmd_Simple(SSD1322_SET_DISPLAY_ON_TEST);
	SendCmd_Simple(SSD1322_SET_DISPLAY_ON_NORMAL);
	//todo: check
	if (0)
		UpdateGrayScaleTable();
}

void SSD1322_SetCmdLock(_Bool state) {
	uint8_t val = state ? 0x16 : 0x12;
	SendCmd_One(SSD1322_SET_COMMAND_LOCK, val);
}

void SSD1322_SetColumnRange(uint8_t start, uint8_t end) {
	if (!end || (start >= end) || (end > COLUMN_MAX))
		return;
	SendCmd_Two(SSD1322_SET_COLUMN_START_END, start, end);
}

void SSD1322_SetRowRange(uint8_t start, uint8_t end) {
	if (!end || (start >= end) || (end > ROW_MAX))
		return;
	SendCmd_Two(SSD1322_SET_ROW_START_END, start, end);
}

void SSD1322_ClearDisplay(void) {
	const uint8_t fill = 0;
	uint32_t i = (COLUMN_MAX+1)*(ROW_MAX+1);
	SendCmd_Two(SSD1322_SET_COLUMN_START_END, 0, COLUMN_MAX);
	SendCmd_Two(SSD1322_SET_ROW_START_END, 0, ROW_MAX);
	CS(true);
	CMD(SSD1322_WRITE_RAM);
	while (i--) {
		Data(&fill, 1);
	}
	CS(false);
}

void SSD1322_DrawSurface(const uint8_t *line, const size_t height, const size_t bytesPerLine) {
    uint32_t i = 0;
    uint32_t j = 0;
    const uint8_t offset = 27;
    uint8_t buffer[bytesPerLine];
	SSD1322_SetColumnRange(offset +1, offset + height);
	SSD1322_SetRowRange(0, 70);

	SendCmd_Simple(SSD1322_WRITE_RAM);
	CS(true);
	for (i = 0; i < height; i++) {
		for (j = 0; j < bytesPerLine; j++) {
			buffer[j] = line[i*bytesPerLine + j];
		}
		Data(buffer, bytesPerLine);
	}
	CS(false);
}

static void UpdateGrayScaleTable(void) {
	uint8_t i;
	const uint8_t grayScaleMax = 180;
	const uint8_t grayScaleCount = 15;
	CS(true);
	CMD(SSD1322_SET_GRAY_SCALE_TABLE);
	for (i = 0; i < grayScaleCount; i++) {
		uint8_t gamma = grayScaleMax/grayScaleCount*i;
		Data(&gamma, 1);
	}
	CS(false);
}

static void SendCmd_Simple(uint8_t cmd) {
	CS(true);
	CMD(cmd);
	CS(false);
}

 void SendCmd_One(uint8_t cmd, uint8_t val) {
	CS(true);
	CMD(cmd);
	Data(&val, 1);
	CS(false);
}

static void SendCmd_Two(uint8_t cmd, uint8_t valA, uint8_t valB) {
	CS(true);
	CMD(cmd);
	Data(&valA, 1);
	Data(&valB, 1);
	CS(false);
}
