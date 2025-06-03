#include "edge_detection.h"

void image_to_grayscale(uint16_t *image_input, uint8_t *image_output, uint8_t width, uint8_t height){
    for(uint32_t i = 0; i < width * height; i++){
        uint16_t pixel = image_input[i];

        uint8_t r = (pixel >> 11) & 0x1F;
        uint8_t g = (pixel >> 5) & 0x3F;
        uint8_t b = (pixel >> 0) & 0x1F;

        r = (r << 3) | (r >> 2);
        g = (g << 2) | (g >> 4);
        b = (b << 3) | (b >> 2);

        uint8_t gray = (uint8_t)(0.299f * r + 0.587f * g + 0.114f * b);

        image_output[i] = gray;
    }
}

void canny(){

}

void grayscale_to_rgb565(uint8_t *grayscale_image, uint16_t *rgb565_output, uint8_t width, uint8_t height){
    for(uint32_t i = 0; i < width*height; i++){
        uint8_t gray = grayscale_image[i];

        uint8_t r = gray >> 3;
        uint8_t g = gray >> 2;
        uint8_t b = gray >> 2;

        rgb565_output[i] = (r << 11) | (g << 5) | b;
    }
}

