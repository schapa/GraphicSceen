

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "bsp.h"
#include "system.h"
#include "ssd1322.h"
#include "gfx.hpp"

void drawSurface(uint8_t **line, uint16_t heigth, uint8_t bytesPerLine);

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	BSP_Init();
	SSD1322_ClearDisplay();

	GfxLayer baseLayer(ColorDepth_4, 256, 64);

	TextWidget timeWdt(FONT_DIGITAL_7SEGMENT, 18, "0123456789");
	TextWidget testWdt(FONT_CENTURY_SCOOLBOOK, 12, "H Hello [xxxx]");
	TextWidget infoWdt(FONT_CENTURY_SCOOLBOOK, 12, "The quick brown fox jumps over the lazy dog");

	timeWdt.setSurface(new GfxSurface(ColorDepth_4, 256, 20));
	testWdt.setSurface(new GfxSurface(ColorDepth_4, 128, 20));
	infoWdt.setSurface(new GfxSurface(ColorDepth_4, 256, 20));

	timeWdt.setVisible(true);
	testWdt.setVisible(true);
	infoWdt.setVisible(true);

	baseLayer.addWidget(&timeWdt);
	baseLayer.addWidget(&testWdt);
	baseLayer.addWidget(&infoWdt);

	testWdt.getShape()->setX(20);
	testWdt.getShape()->setY(20);
	infoWdt.getShape()->setY(40);

	char buffer[128];
	while(1) {
		uint32_t render = System_getUptimeMs();
		baseLayer.render();
		uint32_t sender = System_getUptimeMs();
		render = abs(render - sender);
		drawSurface(baseLayer.getFrameBuffer(),
				baseLayer.getHeigth(), baseLayer.getBytesPerLine());
		sender = abs(sender-System_getUptimeMs());
		sprintf(buffer, "Hello [%4lu.%4lu] %lu FPS ", render, sender, 1000/(render+sender));
		testWdt.setText(buffer);
		timeWdt.setText(timeWdt.getText());
		infoWdt.setText(infoWdt.getText());
	}

	return 0;
}

void drawSurface(uint8_t **line, uint16_t heigth, uint8_t bytesPerLine) {
    uint32_t i = 0;
    uint32_t j = 0;
    const uint8_t offset = 27;
    uint8_t buffer[128];
	SSD1322_SetColumnRange(offset +1, offset + heigth);
	SSD1322_SetRowRange(0, 70);

	OLED_CsSpi(true);
	OLED_CmdSpi(SSD1322_WRITE_RAM);
	for (i = 0; i < heigth; i++) {
		for (j = 0; j < bytesPerLine; j++) {
			buffer[j] = line[i][j];
		}
		OLED_WriteSpi(buffer, bytesPerLine);
	}
	OLED_CsSpi(false);
}
