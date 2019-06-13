
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

#include "lvgl.h"

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

//static uint8_t s_fb[SCREEN_BYTES_PERLINE * SCREEN_HEIGHT];
static uint8_t *s_fb;

static void lvgl_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t *color_p) {
	uint16_t *fb = (uint16_t*)s_fb;
    for (int32_t y = y1; y <= y2; y++) {
        for (int32_t x = x1; x <= x2; x++) {
        	uint16_t *pt = (uint16_t*)color_p++;
        	fb[y * SCREEN_WIDTH + x] = *pt;
        }
    }
	lv_flush_ready();
}

static void guiInit(void) {
	s_fb = BSP_SDRAM_GetBase();
	lv_init();
	lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv);
	disp_drv.disp_flush = lvgl_flush;
	lv_disp_drv_register(&disp_drv);
}

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	Timer_init(onTimerPush);
	_Bool status = BSP_Init();
	DBGMSG_INFO("\nStart sss. Init %d", status);

	STMPE811 touch(BSP_GetHandleI2C_3());
	touch.init();
	L3GD20 accel(BSP_GetHandleSpi_5());

	DiscoLCDInit();
	DiscoLCDInitLayer(0, BSP_SDRAM_GetBase());

	s_accelTim = Timer_newArmed(TICKS_PER_SECOND/3, true, onTimerFire, &accel);

	size_t startS = 0;
	size_t endS = 0;

	W25Q64 flash(BSP_GetHandleSpi_5(), (Gpio_e)GPIO_USER_16);

	guiInit();
	lv_obj_t *label1 = lv_label_create(lv_scr_act(), NULL);
	lv_obj_t *label2 = lv_label_create(lv_scr_act(), NULL);
	lv_label_set_text(label1, "Hellos world!");
	lv_label_set_text(label2, "sdf sdf!");
	lv_obj_align(label1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
	lv_obj_align(label2, NULL, LV_ALIGN_CENTER, +50, 0);

	bool rend = true;
	while(1) {
		lv_task_handler();
		if (rend) {
#ifdef EMULATOR
		SSD1322_DrawSurface(s_fb, SCREEN_HEIGHT, SCREEN_BYTES_PERLINE);
#endif
		}
		Event_t event;
		startS = System_getUptime()*1000 + System_getUptimeMs();
		size_t timeProcess = startS - endS;
		rend = EventQueue_Pend(&event);
		endS = System_getUptime()*1000 + System_getUptimeMs();
		size_t timePend = endS - startS;
		size_t period = timeProcess + timePend;
		int load = period ? 100*timeProcess/(timeProcess + timePend) : 100;
		{
			char buff[256];
			snprintf(buff, sizeof(buff), "Avg. Load %02d%%. Proc %d.%03d. Pend %d.%03d",
					load, timeProcess/1000, timeProcess%1000, timePend/1000, timePend%1000);
//			text->setText(buff);
	    	lv_label_set_text(label1, buff);
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
				const int pin = reinterpret_cast<long>(event.data) >> 1;
				const bool state = reinterpret_cast<long>(event.data) & 1;
				if (pin == GPIO_TOUCH_INT) {
					touch.read();
					char buff[256];
					snprintf(buff, sizeof(buff), "X:Y  %d      %d", touch.getX(), touch.getY());
//					info->setText(buff);
				} else if (pin == GPIO_KEY_WAKE_USER) {
					static int val = 0;
					char buff[256];
					snprintf(buff, sizeof(buff), "EXTI %d", val++);
//					info->setText(buff);
				} else
					DBGMSG_INFO("EXTI: %d %d", pin, state);
				break;
			}
			case EVENT_TIMCALL:
				if (s_accelTim == (long)event.data) {
					accel.read();
					char buff[256];
//					snprintf(buff, sizeof(buff), "Acc \t\t %d \t\t %d \t\t %d",
//							accel.getX(), accel.getY(), accel.getZ());
					snprintf(buff, sizeof(buff), "Acc \t\t 0x%04X \t\t 0x%04X \t\t 0x%04X",
							accel.getX(), accel.getY(), accel.getZ());
//					info->setText(buff);
				} else
					Timer_onTimerCb((long)event.data);
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


