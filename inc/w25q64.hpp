/*
 * w25q64.h
 *
 *  Created on: Jan 28, 2019
 *      Author: shapa
 */

#pragma once

#include <stddef.h>
#include "bspGpio.h"

#include "bsp.h"
//struct SPI_HandleTypeDef;

class W25Q64 { // todo: w25qxxx?
public:
    explicit W25Q64(SPI_HandleTypeDef *spi, Gpio_e pin): ready(false), spi(spi), pin(pin) {};
    virtual ~W25Q64() {};

    bool inline init() { return ready ? true : (ready = initHw()); }

    bool read(size_t start, void *dest, size_t size);
    bool write(size_t start, void *dest, size_t size);
    bool erase();
private:
    bool ready;
    SPI_HandleTypeDef *const spi;
    const Gpio_e pin;

    bool initHw();
};
