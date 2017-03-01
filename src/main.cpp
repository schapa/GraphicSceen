

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
#include "widgetTrip.hpp"

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

#if !defined(EMULATOR) && 0
	GfxLayer baseLayer(PixelFormat_RGB565, 240, 64);
	DiscoLCDInit(baseLayer.getFrameBuffer());
#else
	GfxLayer baseLayer(PixelFormat_GrayScale, 256, 64);
#endif

	TextWidget avgMpg(FONT_LIBEL_SUIT, 16, "AVG.  MPG");
	TextWidget perType(FONT_LIBEL_SUIT, 16, "100kmi/l");
//	TextWidget avgMpg(FONT_LIBEL_SUIT, 16, "A");
//	TextWidget perType(FONT_LIBEL_SUIT, 16, "B");
//	avgMpg.setNegative(true);
//	perType.setNegative(true);

	avgMpg.setSpacing(0);
	avgMpg.createSurface();
	avgMpg.setX(132);
	avgMpg.setY(10);
	avgMpg.setVisible(true);

	perType.setSpacing(0);
	perType.createSurface();
	perType.setX(132);
	perType.setY(28);
	perType.setVisible(true);

	TemperatureWidget temperature;
	TripWidget trip;
	GfxMulti7SegShape clock(4);

	temperature.createSurface();
	temperature.setVisible(true);
	temperature.setX(0);
	temperature.setY(14);
	temperature.setTemperature(888);

	trip.createSurface();
	trip.setVisible(true);
	trip.setX(62);
	trip.setY(14);
	trip.setValue(888);

	clock.createSurface();
	clock.setValue(1888);
	clock.setVisible(true);
	clock.setX(194);
	clock.setY(14);

	baseLayer.addShape(&temperature);
	baseLayer.addShape(&trip);
	baseLayer.addShape(&clock);

	baseLayer.addShape(&avgMpg);
	baseLayer.addShape(&perType);


	while(1) {
		size_t start = System_getUptimeMs();
		baseLayer.render();
		size_t end = System_getUptimeMs() - start;
		DBGMSG_INFO("rend %d", end);
		SSD1322_DrawSurface(baseLayer.getFrameBuffer(), baseLayer.getHeight(), baseLayer.getBytesPerLine());
		Event_t event;
		EventQueue_Pend(&event);
		switch (event.type) {
			case EVENT_SYSTICK: {
				temperature.setTemperature(end);
				temperature.setTemperatureType(System_getUptime() %2);
				trip.setTripInd((System_getUptime() %2), (System_getUptime() %2));
				trip.setValue(999-System_getUptime());
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


