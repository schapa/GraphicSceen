

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
#include "widgetTemperature.hpp"

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

#ifdef EMULATOR
	const uint16_t width = 256;
	const PixelFormat pf = PixelFormat_GrayScale;
#else
	const uint16_t width = 240;
	const PixelFormat pf = PixelFormat_RGB565;
#endif
	const uint16_t heigth = 64;

//	GfxLayer baseLayer(PixelFormat_GrayScale, 256, 64);
	GfxLayer baseLayer(pf, width, heigth);

	DiscoLCDInit(baseLayer.getFrameBuffer());

//	TextWidget infoWdt(FONT_CENTURY_SCOOLBOOK, 12, "The quick brown fox jumps over the lazy dog");

	TemperatureWidget test7;
	test7.createSurface();
	test7.setVisible(true);
	test7.setX(0);
	test7.setY(3);
	baseLayer.addShape(&test7);
	test7.setTemperature( 888);


	while(1) {
		size_t start = System_getUptimeMs();
		baseLayer.render();
		DBGMSG_INFO("rend %d", System_getUptimeMs() - start);
		SSD1322_DrawSurface(baseLayer.getFrameBuffer(), baseLayer.getHeigth(), baseLayer.getBytesPerLine());
		Event_t event;
		EventQueue_Pend(&event);
		switch (event.type) {
			case EVENT_SYSTICK: {
				test7.setTemperature(System_getUptime());
				test7.setTemperatureType(System_getUptime() %2);
				test7.setTripInd((System_getUptime() %2), (System_getUptime() %2));
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


