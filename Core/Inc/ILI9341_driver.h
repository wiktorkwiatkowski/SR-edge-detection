#ifndef ILI9341_DRIVER_H
#define ILI9341_DRIVER_H

#include "stm32f4xx_hal.h"
#include "spi.h"
#include "main.h"


#define ILI9341_WIDTH 240
#define ILI9341_HEIGHT 420



void ILI9341_Write_Command(uint8_t cmd);
void ILI9341_Write_Data(uint8_t data);


#endif