#include "OV7670_driver/OV7670.h"
#include "OV7670_driver/OV7670_reg.h"

/*** Internal Const Values, Macros ***/
// #define OV7670_QVGA_WIDTH 320
// #define OV7670_QVGA_HEIGHT 240
#define SLAVE_ADDR 0x42
// #define FRAME_SIZE    (OV7670_QVGA_WIDTH * OV7670_QVGA_HEIGHT)

// uint32_t frame_buffer[FRAME_SIZE / 4];

/*** Internal Static Variables ***/
static DCMI_HandleTypeDef* s_hdcmi;
static DMA_HandleTypeDef* s_hdma_dcmi;
static I2C_HandleTypeDef* s_hi2c;
static uint32_t s_destAddressForContiuousMode;
static HAL_StatusTypeDef OV7670_write(uint8_t reg_addr, uint8_t data);
static HAL_StatusTypeDef OV7670_read(uint8_t reg_addr, uint8_t* data);
static void OV7670_write_config();

static HAL_StatusTypeDef OV7670_write(uint8_t reg_addr, uint8_t data) {
    HAL_StatusTypeDef ret;
    do {
        ret = HAL_I2C_Mem_Write(s_hi2c, SLAVE_ADDR, reg_addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    } while (ret != HAL_OK && 0);
    return ret;
}

static HAL_StatusTypeDef OV7670_read(uint8_t regAddr, uint8_t* data) {
    HAL_StatusTypeDef ret;
    do {
        ret = HAL_I2C_Master_Transmit(s_hi2c, SLAVE_ADDR, &regAddr, 1, 100);
        // ret | jeśli coś poszło nie tak w jednym albo drugim to będzie error
        ret = ret | HAL_I2C_Master_Receive(s_hi2c, SLAVE_ADDR, data, 1, 100);
    } while (ret != HAL_OK && 0);
    return ret;
}

HAL_StatusTypeDef OV7670_init(DCMI_HandleTypeDef* hdcmi, DMA_HandleTypeDef* hdma_dcmi,
                              I2C_HandleTypeDef* hi2c) {
    s_hdcmi = hdcmi;
    s_hdma_dcmi = hdma_dcmi;
    s_hi2c = hi2c;
    s_destAddressForContiuousMode = 0;

    HAL_GPIO_WritePin(CAMERA_RESET_GPIO_Port, CAMERA_RESET_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(CAMERA_RESET_GPIO_Port, CAMERA_RESET_Pin, GPIO_PIN_SET);
    HAL_Delay(100);

    OV7670_write(0x12, 0x80);  // reset to default values
    HAL_Delay(30);

    uint8_t buffer[1];
    OV7670_read(0x0b, buffer);
    printf("[OV7670] dev id = 0x%X \r\n", buffer[0]);
    if (buffer[0] != 0x73)
        return HAL_ERROR;
    else
        OV7670_write_config(s_hi2c);
    return HAL_OK;
}

static void OV7670_write_config() {
    OV7670_write(0x12, 0x80);  // RESET
    HAL_Delay(30);
    for (int i = 0; OV7670_reg[i][0] != EOC; i++) {
        OV7670_write(OV7670_reg[i][0], OV7670_reg[i][1]);
        HAL_Delay(1);
    }
}

HAL_StatusTypeDef OV7670_start_capture(uint32_t buffor) {
    printf("Adres buforu start capture: 0x%lX\r\n", buffor);
    if(HAL_DCMI_Start_DMA(s_hdcmi, DCMI_MODE_CONTINUOUS, buffor, (OV7670_QVGA_HEIGHT * OV7670_QVGA_WIDTH) / 2) == HAL_OK){
        printf("DCMI DMA started\r\n");
    }else{
        printf("failed\r\n");
    }
    return HAL_OK; 
}