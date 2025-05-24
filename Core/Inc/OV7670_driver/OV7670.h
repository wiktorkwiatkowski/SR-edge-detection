#ifndef OV7670_H
#define OV7670_H

#include "i2c.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#define OV7670_QVGA_WIDTH 320
#define OV7670_QVGA_HEIGHT 240

HAL_StatusTypeDef OV7670_init(DCMI_HandleTypeDef *hdcmi, DMA_HandleTypeDef *hdma_dcmi, I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef OV7670_start_capture(uint32_t buffor);

#endif // __OV7670_H
