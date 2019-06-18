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
#include <assert.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#include "emu.h"
#include "bsp.h"
#include "system.h"
#include "ssd1322.h"
#include "dbg_trace.h"
#include "bspGpio.h"
#ifdef LINUX
#include <sys/prctl.h> // prctl
#endif

static void* sysTickThread (void *arg);
static void* uiThread (void *arg);
static void* eventThread (void *arg);


static void startEventThread(void);
static void startSysTickThread(void);
static void startUiThread(void);

static struct {
	uint8_t *fb;
	size_t offset;
	_Bool selected;
	_Bool ram;
	_Bool mmaped;

	uint32_t touchX;
	uint32_t touchY;
} s_lcd;

static struct {
	int listenSock;
	int acceptSock;
} s_sock;

static uint8_t s_sdramBuff[8*0x400*0x400];

extern "C" void BSP_LcdReset(const _Bool state) {

}
extern "C" void BSP_LcdCs(const _Bool state) {
	s_lcd.selected = state;
}
extern "C" void BSP_LcdWrite(const uint8_t *buff, uint16_t size) {
	if (size < 4)
		return;
	if (s_lcd.mmaped && s_lcd.selected && s_lcd.ram) {
		memcpy((void*)&s_lcd.fb[s_lcd.offset], (void*)buff, size);
		s_lcd.offset += size;
	}
}
extern "C" void BSP_LcdCmd(const uint8_t val) {
	if (val == SSD1322_WRITE_RAM) {
		s_lcd.offset = 0;
		s_lcd.ram = true;
	} else
		s_lcd.ram = false;
}

extern "C" _Bool BSP_Init(void) {
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
	startEventThread();
	startUiThread();
	startSysTickThread();
	System_setStatus(INFORM_LowBatt);

	return true;
}

extern "C" SPI_HandleTypeDef *BSP_GetHandleSpi_5(void) {
	static SPI_HandleTypeDef handle;
	return &handle;
}

extern "C" I2C_HandleTypeDef *BSP_GetHandleI2C_3(void) {
	static I2C_HandleTypeDef handle;
	return &handle;
}

extern "C" uint8_t *BSP_SDRAM_GetBase(void) {
	return s_sdramBuff;
}

extern "C" size_t BSP_SDRAM_GetSize(void) {
	return sizeof(s_sdramBuff);
}

extern "C" void BSP_Gpio_Init(void) {

}
extern "C" void BSP_Gpio_Init_Pin(const Gpio_e pin) {

}
extern "C" const _Bool BSP_Gpio_ReadPin(const Gpio_e pin) {
	return true;
}
extern "C" void BSP_Gpio_SetPin(const Gpio_e pin, const _Bool val) {

}
extern "C" const GpioCfg_t *const BSP_Gpio_CfgGet(const Gpio_e pin) {
	return NULL;
}

static void startEventThread(void) {
	pthread_t pid;
	int result = pthread_create(&pid, NULL, eventThread, NULL);
    if (result) {
    		DBGMSG_ERR("pthread_create()");
    }
}

static void startSysTickThread (void) {

	pthread_t pid;
	int result = pthread_create(&pid, NULL, sysTickThread, NULL);
    if (result) {
    		DBGMSG_ERR("pthread_create()");
    }

}

static void startUiThread(void) {

	char path[128];
	snprintf(path, sizeof(path), "/tmp/lcd_%d", 1/*getpid()*/);
	pthread_t pid;
	int result = pthread_create(&pid, NULL, uiThread, (void*)strdup(path));
    if (result) {
    		DBGMSG_ERR("pthread_create()");
    }
}
#ifdef LINUX
extern int usleep (long val);
#endif

extern "C" void SysTick_Handler(void);
static void* sysTickThread (void *arg) {
#ifdef LINUX
	prctl(PR_SET_NAME, "sysTick");
#endif
	(void)arg;

	while (true) {
		SysTick_Handler();
		usleep((1000*1000) / TICKS_PER_SECOND);
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
#ifdef LINUX
	prctl(PR_SET_NAME, "uiThread");
#endif
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

static void* eventThread (void *arg) {

#ifdef LINUX
	prctl(PR_SET_NAME, "eventThread");
#endif

	do {
		s_sock.listenSock = socket(AF_INET, SOCK_STREAM, 0);
		if (s_sock.listenSock < 0) {
			DBGMSG_ERR("socket creation");
			sleep(1);
		}
	} while (s_sock.listenSock < 0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(BUTTONS_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(s_sock.listenSock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		DBGMSG_ERR("bind");
		exit(-2);
	}

	if (listen(s_sock.listenSock, 1)){
		DBGMSG_ERR("listen");
		exit(-2);
	}

	while(1) {
		s_sock.acceptSock = accept(s_sock.listenSock, NULL, NULL);
		if (s_sock.acceptSock < 0) {
			DBGMSG_ERR("accept");
			continue;
		}

		while(1) {
			char buf[1024];
			int bytes_read = recv(s_sock.acceptSock, buf, 1024, 0);
			if(bytes_read <= 0)
				break;
			buf[bytes_read] = '\0';
			if (bytes_read == 4) {
				uint32_t val = *((uint32_t*)buf);
				EventQueue_Push(EVENT_EXTI, (void*)val, NULL);
			} else if (bytes_read == 12) {
				uint32_t *buff = (uint32_t*)buf; // 2 - press, 3 - release, 4 - drag, [1] - x, [2] - y

				s_lcd.touchX = buff[1];
				s_lcd.touchY = buff[2];
				EventQueue_Push(EVENT_EXTI, (void*)(GPIO_TOUCH_INT << 1), NULL);
			} else {
				DBGMSG_ERR("read %d", bytes_read);
			}
		}

		close(s_sock.acceptSock);
	}
	return NULL;
}

#include "STMPE811.hpp"
void STMPE811::init() {(void)iface;}

bool STMPE811::read(bool force) {
	x = s_lcd.touchX;
	y = s_lcd.touchY;
	return false;
}



