

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

#include "DiscoLcd_ili9341.h"
#include "L3GD20.hpp"
#include "STMPE811.hpp"
#include "ssd1322.h"

#include "w25q64.hpp"

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
		L3GD20 *accel = (L3GD20*)data;
		accel->read();
	}
}

#include "layer.hpp"
#include "widgetText.hpp"

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	Timer_init(onTimerPush);
	_Bool status = BSP_Init();
	DBGMSG_INFO("\nStart sss. Init %d", status);

	STMPE811 touch(BSP_GetHandleI2C_3());
	L3GD20 accel(BSP_GetHandleSpi_5());
	touch.init();

#ifdef EMULATOR
	const PixelFormat pixfmt = PixelFormat_GrayScale;
#else
	const PixelFormat pixfmt = PixelFormat_RGB565;
#endif
	GfxLayer *baseLayer = new GfxLayer(pixfmt, SCREEN_WIDTH, SCREEN_HEIGHT);
	GfxLayer *discoScreenLayer = new GfxLayer(pixfmt, 240, 320, false);
	GfxSurface *textSurface = new GfxSurface(PixelFormat_GrayScale, SCREEN_WIDTH, 30);

	uint8_t *fbs[] = {
		BSP_SDRAM_GetBase(),
		BSP_SDRAM_GetBase() + discoScreenLayer->getFrameBufferSize()
	};
	int layNo = 0;

	DiscoLCDInit();
	DiscoLCDInitLayer(0, fbs[0]);
	DiscoLCDInitLayer(1, fbs[1]);

	TextWidget *text = new TextWidget(FONT_LIBEL_SUIT, 16);
	text->setSurface(textSurface);
	text->setX(10);
	text->setY(10);
	text->setVisible(true);
	text->setText("Hello");

	baseLayer->addShape(text);
	discoScreenLayer->addShape(text);

	TextWidget *info = new TextWidget(FONT_LIBEL_SUIT, 16);
	info->setSurface(new GfxSurface(PixelFormat_GrayScale, SCREEN_WIDTH, 30));
	info->setX(10);
	info->setY(40);
	info->setVisible(true);
	info->setText("ssss");

	baseLayer->addShape(info);
	discoScreenLayer->addShape(info);


	s_accelTim = Timer_newArmed(TICKS_PER_SECOND/3, true, onTimerFire, &accel);

	size_t startS = 0;
	size_t endS = 0;

	W25Q64 flash(BSP_GetHandleSpi_5(), (Gpio_e)GPIO_USER_16);

	bool rend = true;
	while(1) {
		if (rend) {
#ifdef EMULATOR
		SSD1322_DrawSurface(baseLayer->getFrameBuffer(), baseLayer->getHeight(), baseLayer->getBytesPerLine());
#endif
			discoScreenLayer->setFrameBuffer(fbs[layNo], false);
			discoScreenLayer->render();
			DiscoLCDSetActiveLayer(layNo);
			layNo = !layNo;
			baseLayer->render(true);
		}
		Event_t event;
		startS = System_getUptime()*1000 + System_getUptimeMs();
		size_t timeProcess = startS - endS;
		rend = EventQueue_Pend(&event);
		endS = System_getUptime()*1000 + System_getUptimeMs();
		size_t timePend = endS - startS;
		int load = 100*timeProcess/(timeProcess + timePend);
		{
			char buff[256];
			snprintf(buff, sizeof(buff), "Avg. Load %02d%%. Proc %d.%03d. Pend %d.%03d",
					load, timeProcess/1000, timeProcess%1000, timePend/1000, timePend%1000);
			text->setText(buff);
			DBGMSG_INFO("%s",buff);
		}

		switch (event.type) {
			case EVENT_SYSTICK: {
			    static int i = 0;

//			    for (int i = 0; i < 128; ++i)
			    do {
			        uint8_t buff[256];
			        if (!flash.read(i * sizeof(buff), buff, sizeof(buff))) {
			            DBGMSG_ERR("failed to read ");
			            break;
			        }
			        char text[sizeof(buff) * 10 * 2];
			        int occup = 0;
			        for (size_t j = 0; j < sizeof(buff)/2; j += 2) {
			            int16_t *ptr = (int16_t*)buff;
			            occup += snprintf(text + occup, sizeof(text) - occup, "%d %d\n\r", ptr[j], ptr[j+1]);
			        }
			        DBGMSG_INFO("%d, (%d) \r\n%s", i, occup, text);
			        i++;
			    } while (0);
				break;
			}
			case EVENT_EXTI: {
				const int pin = reinterpret_cast<int>(event.data) >> 1;
				const bool state = reinterpret_cast<int>(event.data) & 1;
				if (pin == GPIO_TOUCH_INT) {
					touch.read();
					char buff[256];
					snprintf(buff, sizeof(buff), "X:Y  %d      %d", touch.getX(), touch.getY());
					info->setText(buff);
				} else if (pin == GPIO_KEY_WAKE_USER) {
					static int val = 0;
					char buff[256];
					snprintf(buff, sizeof(buff), "EXTI %d", val++);
					info->setText(buff);
				} else
					DBGMSG_INFO("EXTI: %d %d", pin, state);
				break;
			}
			case EVENT_TIMCALL:
				if (s_accelTim == (uint32_t)event.data) {
					accel.read();
					char buff[256];
//					snprintf(buff, sizeof(buff), "Acc \t\t %d \t\t %d \t\t %d",
//							accel.getX(), accel.getY(), accel.getZ());
					snprintf(buff, sizeof(buff), "Acc \t\t 0x%04X \t\t 0x%04X \t\t 0x%04X",
							accel.getX(), accel.getY(), accel.getZ());
					info->setText(buff);
				} else
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


