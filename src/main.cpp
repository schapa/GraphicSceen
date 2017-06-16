

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
#include "memman.h"

#include "STMPE811.hpp"
#include "ssd1322.h"

#include "dbg_base.h"
#if 01
#include "dbg_trace.h"
#endif


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

#include "layer.hpp"
#include "widgetText.hpp"
extern "C" void DiscoLCDInit(uint8_t *);

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	Timer_init(onTimerPush);
	_Bool status = BSP_Init();
	DBGMSG_INFO("\nStart. Init %d", status);

	STMPE811 touch(BSP_GetHandleI2C_3());
	touch.init();

	GfxLayer *baseLayer = new GfxLayer(PixelFormat_RGB565, 240, 64);
	DiscoLCDInit(baseLayer->getFrameBuffer());

	TextWidget *text = new TextWidget(FONT_LIBEL_SUIT, 16);
	text->setSurface(new GfxSurface(PixelFormat_GrayScale, 240, 30));
	text->setX(10);
	text->setY(10);
	text->setVisible(true);
	text->setText("Hello");

	baseLayer->addShape(text);

	s_accelTim = Timer_newArmed(BSP_TICKS_PER_SECOND/10, true, onTimerFire, NULL);
	s_senseTim = Timer_newArmed(BSP_TICKS_PER_SECOND/5, true, onTimerFire, &touch);

	while(1) {
#ifdef EMULATOR
		SSD1322_DrawSurface(baseLayer->getFrameBuffer(), baseLayer->getHeight(), baseLayer->getBytesPerLine());
#endif
		baseLayer->render();
		Event_t event;
		EventQueue_Pend(&event);
		switch (event.type) {
			case EVENT_SYSTICK: {
				break;
			}
			case EVENT_TIMCALL: {
				Timer_onTimerCb((uint32_t)event.data);
				char buff[256];
				snprintf(buff, sizeof(buff), "X:Y  %d      %d", touch.getX(), touch.getY());
				text->setText(buff);
				break;
			}
			case EVENT_CAN:
				CAN_handleEvent(&event);
				break;
			default:
				break;
		}
		EventQueue_Dispose(&event);
	}

	return 0;
}


