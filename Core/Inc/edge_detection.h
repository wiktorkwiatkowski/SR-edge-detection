#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include <stdint.h>
#include <math.h>

void image_to_grayscale(uint16_t *image_input, uint8_t *image_output, uint8_t width, uint8_t height);

void canny();

void grayscale_to_rgb565(uint8_t *grayscale_image, uint16_t *rgb565_output, uint8_t width, uint8_t height);

#endif