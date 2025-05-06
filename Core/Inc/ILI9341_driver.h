#ifndef ILI9341_DRIVER_H
#define ILI9341_DRIVER_H

#include "main.h"
#include "spi.h"
#include "stm32f4xx_hal.h"

#define ILI9341_WIDTH 240
#define ILI9341_HEIGHT 420

#define ILI9341

void ILI9341_write_command(uint8_t cmd);
void ILI9341_write_data(uint8_t data);
void ILI9341_init();
void ILI9341_set_address(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2);
void ILI9341_draw_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t* image);

#endif