#ifndef ILI9341_DRIVER_H
#define ILI9341_DRIVER_H

#include "stm32f4xx_hal.h"
#include "spi.h"
#include "main.h"


#define ILI9341_WIDTH 240
#define ILI9341_HEIGHT 420

#define ILI9341


void ILI9341_write_command(uint8_t cmd);
void ILI9341_write_data(uint8_t data);
void ILI9341_init();


#endif