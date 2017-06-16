

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

#include "SubiClcokScreen.hpp"

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

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	Timer_init(onTimerPush);
	_Bool status = BSP_Init();
	DBGMSG_INFO("\nStart. Init %d", status);

	STMPE811 touch(BSP_GetHandleI2C_3());
	touch.init();

	s_accelTim = Timer_newArmed(BSP_TICKS_PER_SECOND/10, true, onTimerFire, NULL);
	s_senseTim = Timer_newArmed(BSP_TICKS_PER_SECOND/5, true, onTimerFire, &touch);

	SubiClcokScreen *subi = new SubiClcokScreen(false);

	while(1) {
		const size_t start = System_getUptimeMs();
		subi->draw();
		const size_t end = System_getUptimeMs() - start;
		Event_t event;
		EventQueue_Pend(&event);
		switch (event.type) {
			case EVENT_SYSTICK: {
				subi->setTemperature(end);
				subi->setTemperatureType(System_getUptime() %2);
				subi->setTripInd((System_getUptime() %2), (System_getUptime() %2));
				subi->setValue(999-System_getUptime());
				subi->setType((MPGWidget::Type)(System_getUptime()%3));
				subi->setTime(System_getUptime()/2,System_getUptime()*2);
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


