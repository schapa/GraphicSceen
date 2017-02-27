

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
#include "widgetText.hpp"
#include "memman.h"
#include "widget7Seg.hpp"

#include "dbg_base.h"
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

//	TextWidget infoWdt(FONT_CENTURY_SCOOLBOOK, 12, "The quick brown fox jumps over the lazy dog");
//	TextWidget timeWdt(FONT_DIGITAL_7SEGMENT, 18, "0123456789");
//	TextWidget testWdt(FONT_CENTURY_SCOOLBOOK, 12, "H Hello [xxxx]");

	SegmentDisplayWidget test7;
	test7.setSurface(new GfxSurface(PixelFormat_GrayScale, 100, 64));
	test7.setVisible(true);
	test7.setX(3);
	test7.setY(5);
	baseLayer.addShape(&test7);


//	infoWdt.setSurface(new GfxSurface(PixelFormat_GrayScale, 240, 20));
//	timeWdt.setSurface(new GfxSurface(PixelFormat_GrayScale, 240, 20));
//	testWdt.setSurface(new GfxSurface(PixelFormat_GrayScale, 128, 20));
//	infoWdt.setVisible(true);
//	timeWdt.setVisible(true);
//	testWdt.setVisible(true);

//	baseLayer.addWidget(&infoWdt);
//	baseLayer.addWidget(&timeWdt);
//	baseLayer.addWidget(&testWdt);

//	infoWdt.getShape()->setY(40);
//	testWdt.getShape()->setX(20);
//	testWdt.getShape()->setY(20);

	while(1) {		
		baseLayer.render();
		SSD1322_DrawSurface(baseLayer.getFrameBuffer(), baseLayer.getHeigth(), baseLayer.getBytesPerLine());
		Event_t event;
		EventQueue_Pend(&event);
		switch (event.type) {
			case EVENT_SYSTICK: {
				test7.setValue(-System_getUptime() - 97);
//				char buffer[128];
//				sprintf(buffer, "Uptime is %lu", System_getUptime());
//				testWdt.setText(buffer);
//				static uint32_t StdId = 0x50;
//				CanMsg_t msg = {
//						StdId++,
//						false,
//						false,
//						8,
//						{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
//				};
//				_Bool stat = CAN_write(&msg);
//				DBGMSG_INFO("Send Can %d. %d", StdId-1, stat);
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
	}

	return 0;
}


