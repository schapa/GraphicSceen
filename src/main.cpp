

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diag/Trace.h"
#include "canWrapper.h"

#include "bsp.h"
#include "Queue.h"
#include "system.h"
#include "ssd1322.h"

#include "surface.hpp"
#include "layer.hpp"

#include "shape.hpp"
#include "textWidget.hpp"
#include "dbg_base.h"
#include "memman.h"
#include "sprite.hpp"

#if 0
#include "dbg_trace.h"
#endif


extern "C" void DiscoLCDInit(uint8_t *);
extern "C" void DiscoLCD_setState(_Bool state);

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	_Bool status = BSP_Init();
	SSD1322_ClearDisplay();
	DBGMSG_INFO("\nStart. Init %d", status);

	GfxLayer baseLayer(PixelFormat_GrayScale, 256, 64);

	DiscoLCDInit(baseLayer.getFrameBuffer());

	TextWidget timeWdt(FONT_DIGITAL_7SEGMENT, 18, "0123456789");
	TextWidget testWdt(FONT_CENTURY_SCOOLBOOK, 12, "H Hello [xxxx]");
	TextWidget infoWdt(FONT_CENTURY_SCOOLBOOK, 12, "The quick brown fox jumps over the lazy dog");

	timeWdt.setSurface(new GfxSurface(PixelFormat_GrayScale, 240, 20));
	testWdt.setSurface(new GfxSurface(PixelFormat_GrayScale, 128, 20));
	infoWdt.setSurface(new GfxSurface(PixelFormat_GrayScale, 240, 20));

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
		Event_t event;
		EventQueue_Pend(&event);
		switch (event.type) {
			case EVENT_SYSTICK: {
				sprintf(buffer, "Uptime is %lu", System_getUptime());
				testWdt.setText(buffer);
				static uint32_t StdId = 0x50;
				CanMsg_t msg = {
						StdId++,
						false,
						false,
						8,
						{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
				};
				_Bool stat = CAN_write(&msg);
				DBGMSG_INFO("Send Can %d. %d", StdId-1, stat);
				break;
			}
			case EVENT_CAN:
//				if (event.subType.can == ES_CAN_RX) {
//					CanRxMsgTypeDef *rx = event.data.can.rxMsg;
//					if (rx->StdId == 0x135) {
//						sprintf(buffer, "Rx Can %d on %lu", rx->Data[0], System_getUptime());
//						infoWdt.setText(buffer);
//					}
//				}
				CAN_handleEvent(&event);
				break;
			default:
				break;
		}
		EventQueue_Dispose(&event);
		baseLayer.render();

		SSD1322_DrawSurface(baseLayer.getFrameBuffer(), baseLayer.getHeigth(), baseLayer.getBytesPerLine());
	}

	return 0;
}


