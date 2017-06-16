/*
 * bsp.c
 *
 *  Created on: Dec 29, 2016
 *      Author: shapa
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "bsp.h"
#include "system.h"
#include "ssd1322.h"
#include "dbg_trace.h"
#include "bspGpio.h"

extern int pthread_setname_np (pthread_t __target_thread, const char *__name)
	     __THROW __nonnull ((2));

static void* sysTickThread (void *arg);
static void* uiThread (void *arg);

static void startSysTickThread(void);
static void startUiThread(void);

static struct {
	uint8_t *fb;
	size_t offset;
	_Bool selected;
	_Bool ram;
	_Bool mmaped;
} s_lcd;

void BSP_LcdReset(const _Bool state) {

}
void BSP_LcdCs(const _Bool state) {
	s_lcd.selected = state;
}
void BSP_LcdWrite(const uint8_t *buff, uint16_t size) {
	if (size < 4)
		return;
	if (s_lcd.mmaped && s_lcd.selected && s_lcd.ram) {
		memcpy((void*)&s_lcd.fb[s_lcd.offset], (void*)buff, size);
		s_lcd.offset += size;
	}
}
void BSP_LcdCmd(const uint8_t val) {
	if (val == SSD1322_WRITE_RAM) {
		s_lcd.offset = 0;
		s_lcd.ram = true;
	} else
		s_lcd.ram = false;
}

_Bool BSP_Init(void) {
#if 0
	static _Bool lock = false;
	if (!lock) {
		lock = true;
		extern void Reset_Handler(void);
		Reset_Handler();
		exit(0);
		return false;
	}
#endif
	startUiThread();
	startSysTickThread();
	System_setStatus(INFORM_IDLE);

	return true;
}
SPI_HandleTypeDef *BSP_GetHandleSpi_5(void) {
	static SPI_HandleTypeDef handle;
	return &handle;
}
I2C_HandleTypeDef *BSP_GetHandleI2C_3(void) {
	static I2C_HandleTypeDef handle;
	return &handle;
}


void BSP_Gpio_Init(void) {

}
void BSP_Gpio_Init_Pin(const Gpio_e pin) {

}
const GpioCfg_t *const BSP_Gpio_CfgGet(const Gpio_e pin) {
	return NULL;
}

static void startSysTickThread (void) {

	pthread_t pid;
	int result = pthread_create(&pid, NULL, sysTickThread, NULL);
    if (result) {
    	DBGMSG_ERR("pthread_create()");
    }
	pthread_setname_np(pid, "sysTick");
}

static void startUiThread(void) {

	char path[128];
	snprintf(path, sizeof(path), "/tmp/lcd_%d", 1/*getpid()*/);
	pthread_t pid;
	int result = pthread_create(&pid, NULL, uiThread, (void*)strdup(path));
    if (result) {
    	DBGMSG_ERR("pthread_create()");
    }
	pthread_setname_np(pid, "uiThread");
}

static void* sysTickThread (void *arg) {
	(void)arg;

	while (true) {
		extern void SysTick_Handler(void);
		extern int usleep (__useconds_t __useconds);
		SysTick_Handler();
		usleep((1000*1000) / BSP_TICKS_PER_SECOND);
	}
	return NULL;
}

static int mmapPerform(char *path) {
	int fd = -1;
	do {
		fd = open(path, O_RDWR | O_CREAT, S_IRWXG);
		DBGMSG_M("Open [%s] fd %d", path, fd);
		if (fd < 0) {
			usleep((1000*1000));
			continue;
		}
	} while (fd < 0);
	lseek (fd, SCREEN_SIZE + 1, SEEK_SET);
	write (fd, "", 1);
	lseek (fd, 0, SEEK_SET);

	void *p = mmap (0, SCREEN_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		DBGMSG_ERR("mmap");
		close(fd);
		return -1;
	}

	struct stat sb;
	if (fstat(fd, &sb) == -1)  {
		DBGMSG_ERR("fstat");
		close(fd);
		return -1;
	}
	s_lcd.fb = (uint8_t*)p;
	return fd;
}

static void* uiThread (void *arg) {

	char path[256];
	int fd = mmapPerform((char*)arg);
	snprintf(path, sizeof(path), "./build/gui %s", (char*)arg);

	DBGMSG_H("Call [%s]", path);
	s_lcd.mmaped = true;
	system(path);
	free(arg);
	munmap((void*)s_lcd.fb, SCREEN_SIZE);
	close(fd);
	exit(0);
	return NULL;

}
