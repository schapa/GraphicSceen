

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diag/Trace.h"
#include "canWrapper.h"

#include "bsp.h"
#include "Queue.h"
#include "system.h"
#include "timers.h"
#include "ssd1322.h"

#include "surface.hpp"
#include "layer.hpp"

#include "shape.hpp"
#include "memman.h"
#include "widgetText.hpp"
#include "widget7Seg.hpp"
#include "widgetTemperature.hpp"
#include "widgetTrip.hpp"
#include "widgetTime.hpp"
#include "widgetMpg.hpp"
#include "STMPE811.hpp"

#include "dbg_base.h"
#if 01
#include "dbg_trace.h"
#endif

extern "C" void DiscoLCDInit(uint8_t *);
extern "C" void DiscoLCD_setState(_Bool state);

static void onTimerPush(uint32_t id) {
	EventQueue_Push(EVENT_TIMCALL, (void*)id, NULL);
}

static uint32_t s_accelTim = 0;
static uint32_t s_senseTim = 0;


static void onTimerFire(uint32_t id, void *data) {
	if (id == s_accelTim) {

	} else if (id == s_senseTim) {
		STMPE811 *touch = reinterpret_cast<STMPE811*>(data);
		uint16_t x, y, z;
		if (touch->read(x, y, z))
			DBGMSG_M("Touch: %d %d %d", x, y, z);
	}
}

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	Timer_init(onTimerPush);
	_Bool status = BSP_Init();
	SSD1322_ClearDisplay();
	DBGMSG_INFO("\nStart. Init %d", status);

	STMPE811 touch(BSP_GetHandleI2C_3());
	touch.init();

	s_accelTim = Timer_newArmed(BSP_TICKS_PER_SECOND/10, true, onTimerFire, NULL);
	s_senseTim = Timer_newArmed(BSP_TICKS_PER_SECOND/5, true, onTimerFire, &touch);

#if !defined(EMULATOR) && 01
	GfxLayer *baseLayer = new GfxLayer(PixelFormat_RGB565, 240, 64);
	DiscoLCDInit(baseLayer->getFrameBuffer());
#else
	GfxLayer *baseLayer = new GfxLayer(PixelFormat_GrayScale, 256, 64);
#endif

	TemperatureWidget *temperature = new TemperatureWidget();
	TripWidget *trip = new TripWidget();
	MPGWidget *mpg = new MPGWidget();
	TimeWidget *clock = new TimeWidget();

	temperature->setVisible(true);
	temperature->setX(0);
	temperature->setY(14);
	temperature->setTemperature(0);

	trip->setVisible(true);
	trip->setX(62);
	trip->setY(14);
	trip->setValue(0);

	mpg->setVisible(true);
	mpg->setX(132);
	mpg->setY(10);

	clock->setVisible(true);
	clock->setX(194);
	clock->setY(14);
	clock->setTime(8,50);

	baseLayer->addShape(temperature);
	baseLayer->addShape(trip);
	baseLayer->addShape(mpg);
	baseLayer->addShape(clock);

	while(1) {
		const size_t start = System_getUptimeMs();
		baseLayer->render();
		const size_t end = System_getUptimeMs() - start;
//		DBGMSG_INFO("rend %d", end);
		SSD1322_DrawSurface(baseLayer->getFrameBuffer(), baseLayer->getHeight(), baseLayer->getBytesPerLine());
		Event_t event;
		EventQueue_Pend(&event);
		switch (event.type) {
			case EVENT_SYSTICK: {
				temperature->setTemperature(end);
				temperature->setTemperatureType(System_getUptime() %2);
				trip->setTripInd((System_getUptime() %2), (System_getUptime() %2));
				trip->setValue(999-System_getUptime());
				mpg->setType((MPGWidget::Type)(System_getUptime()%3));
				clock->setTime(System_getUptime()/2,System_getUptime()*2);

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
			case EVENT_TIMCALL: {
				Timer_onTimerCb((uint32_t)event.data);
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


