/*
 * w25q64.cpp
 *
 *  Created on: Jan 16, 2018
 *      Author: pavelgamov
 */

#include "w25q64.hpp"
#include <assert.h>

enum {
	CMD_PageProgramm = 0x02,
	CMD_ReadData = 0x03,
	CMD_WriteDisable = 0x04,
	CMD_ReadStatusReg = 0x05,
	CMD_WriteEnable = 0x06,
	CMD_EraseChip = 0x60,
	CMD_HighPerformanceMode = 0xA3,
};
#include "dbg_base.h"
#if 01
#include "dbg_trace.h"
#endif

static bool waitOperation(SPI_HandleTypeDef *hspi, Gpio_e pin) {
    uint8_t buff[2];
    int attempts = 100;
    do {
        buff[0] = CMD_ReadStatusReg;
        buff[1] = 0xFF;
        BSP_Gpio_SetPin(pin, false);
        HAL_StatusTypeDef rv = HAL_SPI_TransmitReceive(hspi, buff, buff, 2, 0x0F);
        BSP_Gpio_SetPin(pin, true);
        if (rv != HAL_OK) {
            DBGMSG_ERR("Failed to read status register. Err %d", rv);
            return false;
        }
        if (!(buff[1] & 1))
            return true;
    } while (--attempts > 0);
    DBGMSG_H("Status register didn't set to valid state. Now 0x%02X", buff[1]);
    return false;
}

static bool writeCmd(SPI_HandleTypeDef *hspi, Gpio_e pin,uint8_t cmd) {
    BSP_Gpio_SetPin(pin, false);
    HAL_StatusTypeDef rv = HAL_SPI_Transmit(hspi, &cmd, 1, 0x0F);
    BSP_Gpio_SetPin(pin, true);
    if (rv != HAL_OK)
        DBGMSG_ERR("failed to write command %d. err %d", cmd, rv);
    return (rv == HAL_OK);
}

bool W25Q64::initHw() {
    bool res = false;
    do {
        if (!writeCmd(spi, pin, CMD_HighPerformanceMode))
            break;
        if (!waitOperation(spi, pin))
            break;
        // Read JEDEC ID
        uint8_t buff[] = { 0x9F, 0xFF, 0xFF, 0xFF, };
        BSP_Gpio_SetPin(pin, false);
        HAL_StatusTypeDef rv = HAL_SPI_TransmitReceive(spi, buff, buff, sizeof(buff), 0x0F);
        BSP_Gpio_SetPin(pin, true);
        if ((rv != HAL_OK) || (buff[1] != 0xEF)) {
            DBGMSG_ERR("Failed to get JEDEC ID. Err %d. Val 0x%02X", rv, buff[1]);
            DBGMSG_ERR("0x%02X 0x%02X 0x%02X 0x%02X ", buff[0], buff[1], buff[2], buff[3]);
            break;
        }
        DBGMSG_L("Init successful");
        res = true;
    } while (0);
    return res;
}

bool W25Q64::read(size_t start, void *dest, size_t size) {
    if (!init())
        return false;
    bool res = false;
    do {
    //  uint8_t address[4] = { CMD_ReadData, (startAdr >> 16) & 0xFF, (startAdr >> 8) & 0xFF, (startAdr) & 0xFF };
        uint8_t address[4] = { CMD_ReadData };
        address[1] = (start >> 16);
        address[2] = (start >> 8);
        address[3] = (start & 0xFF);
        BSP_Gpio_SetPin(pin, false);
        HAL_StatusTypeDef rv = HAL_SPI_Transmit(spi, address, sizeof(address), 0x0F);
        if (rv != HAL_OK)
            break;
        rv = HAL_SPI_TransmitReceive(spi, (uint8_t*)dest, (uint8_t*)dest, size, 0x0F);
        if (rv != HAL_OK)
            break;
        res = true;
    } while (0);
    BSP_Gpio_SetPin(pin, true);
    return res;
}

bool W25Q64::write(size_t start, void *dest, size_t size) {
    assert(!(start & 0xFF));
    assert(size < 256);
    if (!init() || !writeCmd(spi, pin, CMD_WriteEnable))
        return false;
    bool res = false;
//    do {
//    //    uint8_t address[4] = { CMD_PageProgramm, (startAdr >> 16) & 0xFF, (startAdr >> 8) & 0xFF, (startAdr) & 0xFF };
//          uint8_t address[4] = { CMD_PageProgramm };
//          address[1] = (start >> 16);
//          address[2] = (start >> 8);
//          address[3] = (start & 0xFF);
//          BSP_Gpio_SetPin(pin, false);
//          HAL_StatusTypeDef rv = HAL_SPI_Transmit(spi, address, sizeof(address), 0x0F);
//          if (rv != HAL_OK)
//              break;
//          rv = HAL_SPI_Transmit(spi, (uint8_t*)dest, size, 0x0F);
//          if (rv != HAL_OK)
//              break;
//    } while (0);
//    BSP_Gpio_SetPin(pin, true);
    return res;

}

bool W25Q64::erase() {
    do {
        if (!writeCmd(spi, pin, CMD_WriteEnable))
            break;
        if (!waitOperation(spi, pin))
            break;
        if (!writeCmd(spi, pin, CMD_EraseChip))
            break;
        if (!waitOperation(spi, pin))
            break;
        return true;
    } while (0);
    return false;
}

