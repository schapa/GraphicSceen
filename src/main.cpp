

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diag/Trace.h"
#include "canWrapper.h"

#include "bsp.h"
#include "system.h"
#include "ssd1322.h"
#include "gfx.hpp"
#include "dbg_base.h"

#if 01
#include "dbg_trace.h"
#endif


extern "C" void DiscoLCDInit(uint8_t *);
extern "C" void DiscoLCD_setState(_Bool state);

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	BSP_Init();
	SSD1322_ClearDisplay();
	DBGMSG_L("hello!");

//	GfxLayer baseLayer(PixelFormat_RGB565, 240, 64);
//
//	DiscoLCDInit(baseLayer.getFrameBuffer());
//
//	TextWidget timeWdt(FONT_DIGITAL_7SEGMENT, 18, "0123456789");
//	TextWidget testWdt(FONT_CENTURY_SCOOLBOOK, 12, "H Hello [xxxx]");
//	TextWidget infoWdt(FONT_CENTURY_SCOOLBOOK, 12, "The quick brown fox jumps over the lazy dog");
//
//	timeWdt.setSurface(new GfxSurface(PixelFormat_RGB565, 240, 20));
//	testWdt.setSurface(new GfxSurface(PixelFormat_RGB565, 128, 20));
//	infoWdt.setSurface(new GfxSurface(PixelFormat_RGB565, 240, 20));
//
//	timeWdt.setVisible(true);
//	testWdt.setVisible(true);
//	infoWdt.setVisible(true);
//
//	baseLayer.addWidget(&timeWdt);
//	baseLayer.addWidget(&testWdt);
//	baseLayer.addWidget(&infoWdt);
//
//	testWdt.getShape()->setX(20);
//	testWdt.getShape()->setY(20);
//	infoWdt.getShape()->setY(40);

	char buffer[128];
	while(1) {
		Event_t event;
		BSP_pendEvent(&event);
		switch (event.type) {
			case EVENT_SYSTICK:
				if (event.subType.systick == ES_SYSTICK_SECOND_ELAPSED) {
//					sprintf(buffer, "Uptime is %lu", System_getUptime());
//					testWdt.setText(buffer);
					static uint32_t StdId = 0x50;
					CanTxMsgTypeDef txMSg = {
							StdId++,
							0,
							CAN_ID_STD, // CAN_ID_EXT
							CAN_RTR_DATA, // CAN_RTR_REMOTE
							8,
							{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }

					};
					CAN_write(&txMSg);
					DBGMSG_INFO("Send Can %d", StdId-1);
					DBGMSG_INFO("Send Can %d", StdId-1);
					DBGMSG_INFO("Send Can %d", StdId-1);
					DBGMSG_INFO("Send Can %d", StdId-1);
					DBGMSG_INFO("Send Can %d", StdId-1);
					DBGMSG_INFO("Send Can %d", StdId-1);
				}
				break;
			case EVENT_CAN:
				if (event.subType.can == ES_CAN_RX) {
					CanRxMsgTypeDef *rx = event.data.can.rxMsg;
					if (rx->StdId == 0x135) {
//						sprintf(buffer, "Rx Can %d on %lu", rx->Data[0], System_getUptime());
//						infoWdt.setText(buffer);
					}
				}
				CAN_handleEvent(&event);
				break;
			default:
				break;
		}
//		baseLayer.render();
	}

	return 0;
}


