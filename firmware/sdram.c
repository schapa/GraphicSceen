/*
 * sdram.c
 *
 *  Created on: Jun 16, 2017
 *      Author: shapa
 */

#include "stm32f4xx_hal.h"
#include <stdbool.h>

static struct {
	 SDRAM_HandleTypeDef dev;
	 uint8_t *base;
	 size_t size;
} s_sdram = {
		.base = (uint8_t*)0xD0000000,
		.size = (8*1024*1024),
};

#define SDRAM_TIMEOUT ((uint32_t)0xFFFF)
#define REFRESH_COUNT ((uint32_t)0x0569) /* SDRAM refresh counter (90Mhz SD clock) */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

static _Bool init(void);
static _Bool sdram_test(void);

_Bool BSP_SDRAM_Init(void) {

	__HAL_RCC_FMC_CLK_ENABLE();

	const FMC_SDRAM_InitTypeDef iface = {
		FMC_SDRAM_BANK2,
		FMC_SDRAM_COLUMN_BITS_NUM_8,
		FMC_SDRAM_ROW_BITS_NUM_12,
		FMC_SDRAM_MEM_BUS_WIDTH_16,
		FMC_SDRAM_INTERN_BANKS_NUM_4,
		FMC_SDRAM_CAS_LATENCY_3,
		FMC_SDRAM_WRITE_PROTECTION_DISABLE,
		FMC_SDRAM_CLOCK_PERIOD_2,
		FMC_SDRAM_RBURST_DISABLE,
		FMC_SDRAM_RPIPE_DELAY_1
	};
	const FMC_SDRAM_TimingTypeDef timing = {
			2,
			7,
			4,
			7,
			2,
			2,
			2
	};
	s_sdram.dev.Init = iface;
	s_sdram.dev.Instance = FMC_SDRAM_DEVICE;

	HAL_StatusTypeDef stat = HAL_SDRAM_Init(&s_sdram.dev, (FMC_SDRAM_TimingTypeDef*)&timing);
	if (stat != HAL_OK)
		return false;
	if (!init())
		return false;
	return sdram_test();
}

uint8_t *BSP_SDRAM_GetBase(void) {
	return s_sdram.base;
}

size_t BSP_SDRAM_GetSize(void) {
	return s_sdram.size;
}

static _Bool init(void) {

	const uint32_t tmpmrd = SDRAM_MODEREG_BURST_LENGTH_2          |
							SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
							SDRAM_MODEREG_CAS_LATENCY_3           |
							SDRAM_MODEREG_OPERATING_MODE_STANDARD |
							SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
	const FMC_SDRAM_CommandTypeDef cmd[] = {
			{ FMC_SDRAM_CMD_CLK_ENABLE, FMC_SDRAM_CMD_TARGET_BANK2, 1, 0 },
			{ FMC_SDRAM_CMD_PALL, FMC_SDRAM_CMD_TARGET_BANK2, 1, 0 },
			{ FMC_SDRAM_CMD_AUTOREFRESH_MODE, FMC_SDRAM_CMD_TARGET_BANK2, 4, 0 },
			{ FMC_SDRAM_CMD_LOAD_MODE, FMC_SDRAM_CMD_TARGET_BANK2, 1, tmpmrd },
	};
	HAL_StatusTypeDef stat = HAL_SDRAM_SendCommand(&s_sdram.dev, (FMC_SDRAM_CommandTypeDef*)&cmd[0], SDRAM_TIMEOUT);
	if (stat != HAL_OK)
		return false;
	HAL_Delay(1);
	for (size_t i = 1; i < sizeof(cmd)/sizeof(*cmd); i++) {
		HAL_SDRAM_SendCommand(&s_sdram.dev, (FMC_SDRAM_CommandTypeDef*)&cmd[i], SDRAM_TIMEOUT);
		if (stat != HAL_OK)
			return false;
	}
	return HAL_SDRAM_ProgramRefreshRate(&s_sdram.dev, REFRESH_COUNT) == HAL_OK;
}

static _Bool sdram_test(void) {
    uint8_t pattern = 0xAA;
    uint8_t antipattern = 0x55;
    uint32_t mem_size = s_sdram.size;
    uint8_t * const mem_base = (uint8_t*)s_sdram.base;

    /* test data bus */
    for (uint8_t i = 1; i; i <<= 1) {
        *mem_base = i;
        if (*mem_base != i)
            return false;
    }

    /* test address bus */
    /* Check individual address lines */
    for (uint32_t i = 1; i < mem_size; i <<= 1) {
        mem_base[i] = pattern;
        if (mem_base[i] != pattern)
            return false;
    }

    /* Check for aliasing (overlapping addresses) */
    mem_base[0] = antipattern;
    for (uint32_t i = 1; i < mem_size; i <<=1 ) {
        if (mem_base[i] != pattern)
            return false;
    }

    /* test all ram cells */
    for (uint32_t i = 0; i < mem_size; i++) {
        mem_base[i] = pattern;
        if (mem_base[i] != pattern)
            return false;
    }

    return true;
}
