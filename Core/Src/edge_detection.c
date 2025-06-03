#include "edge_detection.h"

uint8_t temp_buf1[WIDTH * HEIGHT];
uint8_t temp_buf2[WIDTH * HEIGHT];
uint8_t temp_buf3[WIDTH * HEIGHT];
uint8_t temp_buf4[WIDTH * HEIGHT];


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
        temp_buf1[i] = gray;
    }
}

void grayscale_to_rgb565(uint8_t *grayscale_image, uint16_t *rgb565_output, uint8_t width, uint8_t height){
    for(uint32_t i = 0; i < width*height; i++){
        uint8_t gray = temp_buf2[i];

        uint8_t r = gray >> 3;
        uint8_t g = gray >> 2;
        uint8_t b = gray >> 2;

        rgb565_output[i] = (r << 11) | (g << 5) | b;
    }
}

void canny(){
    smooth();
    count_gradient();
    threshold();
    track_edges();
}

/*temp_buf1 -> original image*/
/*temp_buf2 -> smoothed image*/
void smooth(){
    float matrix[3][3] = {
        {1.0f/16, 2.0f/16, 1.0f/16},
        {2.0f/16, 4.0f/16, 2.0f/16},
        {1.0f/16, 2.0f/16, 1.0f/16}
    };

    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 1; x < WIDTH - 1; x++) {
            float sum = 0.0f;

            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    uint8_t val = temp_buf1[(y + j) * WIDTH + (x + i)];
                    sum += val * matrix[j + 1][i + 1];
                }
            }

            temp_buf2[y * WIDTH + x] = (uint8_t)sum;
        }
    }
}

/*
/*IN-> smoothed (temp_buf2)
/*CALCULATED-> gradient_horizontal, gradient vertical, gradient_direction
/*OUT-> gradient_direction (temp_buf3)
*/
void count_gradient(){
    int Gx[3][3] = {
        {-1,0,1},
        {-2,0,2},
        {-1,0,1}
    };

    int Gy[3][3] = {
        {-1,-2,-1},
        {0,0,0},
        {1,2,1}
    };

    //horizontal gradient
    for(int i = 1; i < HEIGHT - 1; i++){
        for(int j = 1; j < WIDTH - 1; j++){
            int sum = 0;
            for(int x = -1; x <= 1; x++){
                for(int y = -1; y <=1; y++){
                    uint8_t val = temp_buf2[(y + i) * WIDTH + (x + j)];
                    sum += val * Gx[x + 1][y + 1];
                }
            }
            temp_buf1[i * WIDTH + j] = sum;
        }
    }

    //vertical gradient
    for(int i = 1; i < HEIGHT - 1; i++){
        for(int j = 1; j < WIDTH - 1; j++){
            int sum = 0;;
            for(int x = -1; x <= 1; x++){
                for(int y = -1; y <=1; y++){
                    uint8_t val = temp_buf2[(y + i) * WIDTH + (x + j)];
                    sum += val * Gy[x + 1][y + 1];
                }
            }
            temp_buf3[i * WIDTH + j] = sum;
        }
    }

    //magnitude -> temp_buf2
    for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < HEIGHT; x++){
            int gx = (int8_t)temp_buf1[y * WIDTH + x];  // gradient horizontal
            int gy = (int8_t)temp_buf3[y * WIDTH + x];  // gradient vertical
            int magnitude = sqrt(gx * gx + gy * gy);
            if (magnitude > 255) magnitude = 255;
                temp_buf2[y * WIDTH + x] = (uint8_t)magnitude;        }
    }

    //temp_buf1 -> gradient horizontal
    //temp_buf2 -> gradient magnitude
    //temp_buf3 -> gradient vertical


    // suppression
    int gradient_direction = 0;
    double angle = 0.0;

    for(uint8_t y = 0; y < HEIGHT; y++){
        for(uint8_t x = 0; x < WIDTH; x++){
            angle = atan2((double) temp_buf3[y * HEIGHT + x], (double) temp_buf1[y * HEIGHT + x]) * 180 / 3.14;
            gradient_direction = round_angle(angle);
            int basic_index = y * HEIGHT + x;

            if(gradient_direction == 1){
                if(temp_buf2[basic_index] < temp_buf2[y * HEIGHT + x - 1] || temp_buf2[basic_index] < temp_buf2[y * HEIGHT + x + 1]) temp_buf4[basic_index] = 0;
                else temp_buf4[basic_index] = temp_buf2[basic_index];
            }
            else if(gradient_direction == 2){
                if(temp_buf2[basic_index] < temp_buf2[(y - 1) * HEIGHT + x - 1] || temp_buf2[basic_index] < temp_buf2[(y + 1) * HEIGHT + x + 1]) temp_buf4[basic_index] = 0;
                else temp_buf4[basic_index] = temp_buf2[basic_index];
            }
            else if(gradient_direction == 3){
                if(temp_buf2[basic_index] < temp_buf2[(y - 1) * HEIGHT + x] || temp_buf2[basic_index] < temp_buf2[(y + 1) * HEIGHT + x]) temp_buf4[basic_index] = 0;
                else temp_buf4[basic_index] = temp_buf2[basic_index];
            }
            else if(gradient_direction == 4){
                if(temp_buf2[basic_index] < temp_buf2[(y + 1) * HEIGHT + x - 1] || temp_buf2[basic_index] < temp_buf2[(y - 1) * HEIGHT + x + 1]) temp_buf4[basic_index] = 0;
                else temp_buf4[basic_index] = temp_buf2[basic_index];
            }
            else temp_buf4[basic_index] = 0;
        }
    }

}

int round_angle(double angle)
{
    int degrees = angle;
    if((-22.5 <= degrees && degrees < 22.5) || (157.5 <= degrees) || (degrees < -157.5)) degrees = 1;           //zakres: (-180;-157.5)u<-22.5;22.5)u(157.5;180)    -> poziom
    else if((22.5 <= degrees && degrees < 67.5) ||  (-157.5 <= degrees && degrees < -112.5)) degrees = 2;       //zakres: <22.5;67.5)u<-157.5;112.5)                -> skos NE SW
    else if((67.5 <= degrees && degrees < 112.5) ||  (-112.5 <= degrees && degrees < -67.5)) degrees = 3;       //zakres: <67.5;112.5)u<-112.5;-67.5)               -> pion
    else if((112.5 <= degrees && degrees < 157.5) ||  (-67.5 <= degrees && degrees < -22.5)) degrees = 4;       //zakres: <112.5;157.5)u<-67.5;22.5)                -> skos NW SE
    else degrees = 0;
    return degrees;
}

// temp_buf4 -> suppression
// reszta "wolne"

void threshold(){
        for(int y = 0; y < HEIGHT; y++){
        for(int x = 0; x < WIDTH; x++){
            if(temp_buf4[y * HEIGHT + x] <= LOW_THRESHOLD) temp_buf1[y * HEIGHT + x] = 0;
            else if(temp_buf4[y * HEIGHT + x] <= HIGH_THRESHOLD) temp_buf1[y * HEIGHT + x] = 100;
            else if(temp_buf4[y * HEIGHT + x] > HIGH_THRESHOLD) temp_buf1[y * HEIGHT + x] = 255;
            else temp_buf1[y * HEIGHT + x] = 0;
        }
    }

}

// temp_buf1 -> after thresholding
// reszta "wolne"
void track_edges(){
    for(int y = 1; y < HEIGHT - 1; y++){
        for(int x = 1; x < WIDTH - 1; x++){
            if(temp_buf1[y * HEIGHT + x] == 255) temp_buf2[y * HEIGHT + x] = 255;
            else if(temp_buf1[y * HEIGHT + x] == 100){
                if(check_edge( y, x))
                temp_buf2[y * HEIGHT + x] = 255;
                else temp_buf2[y * HEIGHT + x] = 0;
            }
            else temp_buf2[y * HEIGHT + x] = 0;
        }
    }

}

int check_edge(uint8_t index_y, uint8_t index_x){
    int connected = 0;
    for(int col = index_y - 1; col <= index_y + 1; col++){
        for(int row = index_x - 1; row <= index_x + 1; row++){
            if(row == 0 && col == 0) break;
            if(temp_buf1[col * HEIGHT + row] == 255) {
                connected = 1;
                break;
            }
            if(connected) break;
        }
    }
    return connected;
}
