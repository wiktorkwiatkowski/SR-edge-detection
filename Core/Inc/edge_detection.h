#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include <stdint.h>
#include <math.h>

#define WIDTH 80
#define HEIGHT 80
#define HIGH_THRESHOLD 150
#define LOW_THRESHOLD 75

extern uint8_t temp_buf1[WIDTH * HEIGHT];
extern uint8_t temp_buf2[WIDTH * HEIGHT];
extern uint8_t temp_buf3[WIDTH * HEIGHT];
extern uint8_t temp_buf4[WIDTH * HEIGHT];

/**************GREYSCALE TRANSITION***************/
void image_to_grayscale(uint16_t *image_input, uint8_t *image_output, uint8_t width, uint8_t height);
void grayscale_to_rgb565(uint8_t *grayscale_image, uint16_t *rgb565_output, uint8_t width, uint8_t height);

/*******************CANNY STEPS*******************/
void canny();

void smooth();
void count_gradient();
void threshold();
void track_edges();
int round_angle(double angle);
int check_edge(uint8_t index_y, uint8_t index_x);

#endif