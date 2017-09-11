

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "canWrapper.h"
#include "bsp.h"
#include "bspGpio.h"
#include "Queue.h"
#include "system.h"
#include "timers.h"
#include "memman.h"

#include "L3GD20.hpp"
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

static void onTimerFire(uint32_t id, void *data) {
	if (id == s_accelTim) {
//		L3GD20 *accel = (L3GD20*)data;
//		accel->read();
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
	L3GD20 accel(BSP_GetHandleSpi_5());
	touch.init();

#ifdef EMULATOR
	const PixelFormat pixfmt = PixelFormat_GrayScale;
#else
	const PixelFormat pixfmt = PixelFormat_RGB565;
#endif
	GfxLayer *baseLayer = new GfxLayer(pixfmt, SCREEN_WIDTH, SCREEN_HEIGHT);
	DiscoLCDInit(baseLayer->getFrameBuffer());

	TextWidget *text = new TextWidget(FONT_LIBEL_SUIT, 16);
	text->setSurface(new GfxSurface(PixelFormat_GrayScale, SCREEN_WIDTH, 30));
	text->setX(10);
	text->setY(10);
	text->setVisible(true);
	text->setText("Hello");

	baseLayer->addShape(text);

	s_accelTim = Timer_newArmed(BSP_TICKS_PER_SECOND/10, true, onTimerFire, &accel);

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
			case EVENT_EXTI: {
				const int pin = reinterpret_cast<int>(event.data) >> 1;
				const bool state = reinterpret_cast<int>(event.data) & 1;
				if (pin == GPIO_TOUCH_INT) {
					touch.read();
					char buff[256];
					snprintf(buff, sizeof(buff), "X:Y  %d      %d", touch.getX(), touch.getY());
					text->setText(buff);
				} //else
					DBGMSG_INFO("EXTI: %d %d", pin, state);
				break;
			}
			case EVENT_TIMCALL:
				Timer_onTimerCb((uint32_t)event.data);
				break;
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


