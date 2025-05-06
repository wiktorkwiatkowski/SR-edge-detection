#include "ILI9341_driver.h"

void ILI9341_write_command(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi5, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

void ILI9341_write_data(uint8_t data) {
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit(&hspi5, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

void ILI9341_init(void) {
    // SOFTWARE RESET
    ILI9341_write_command(0x01);
    HAL_Delay(1000);

    // POWER CONTROL A
    ILI9341_write_command(0xCB);
    ILI9341_write_data(0x39);
    ILI9341_write_data(0x2C);
    ILI9341_write_data(0x00);
    ILI9341_write_data(0x34);
    ILI9341_write_data(0x02);

    // POWER CONTROL B
    ILI9341_write_command(0xCF);
    ILI9341_write_data(0x00);
    ILI9341_write_data(0xC1);
    ILI9341_write_data(0x30);

    // DRIVER TIMING CONTROL A
    ILI9341_write_command(0xE8);
    ILI9341_write_data(0x85);
    ILI9341_write_data(0x00);
    ILI9341_write_data(0x78);

    // DRIVER TIMING CONTROL B
    ILI9341_write_command(0xEA);
    ILI9341_write_data(0x00);
    ILI9341_write_data(0x00);

    // POWER ON SEQUENCE CONTROL
    ILI9341_write_command(0xED);
    ILI9341_write_data(0x64);
    ILI9341_write_data(0x03);
    ILI9341_write_data(0x12);
    ILI9341_write_data(0x81);

    // PUMP RATIO CONTROL
    ILI9341_write_command(0xF7);
    ILI9341_write_data(0x20);

    // POWER CONTROL VRH
    ILI9341_write_command(0xC0);
    ILI9341_write_data(0x23);

    // POWER CONTROL SAP/BT
    ILI9341_write_command(0xC1);
    ILI9341_write_data(0x10);

    // VCM CONTROL
    ILI9341_write_command(0xC5);
    ILI9341_write_data(0x3E);
    ILI9341_write_data(0x28);

    // VCM CONTROL 2
    ILI9341_write_command(0xC7);
    ILI9341_write_data(0x86);

    // MEMORY ACCESS CONTROL
    ILI9341_write_command(0x36);
    ILI9341_write_data(0x48);  // MX, BGR

    // PIXEL FORMAT
    ILI9341_write_command(0x3A);
    ILI9341_write_data(0x55);  // 16bit/pixel

    // FRAME RATE CONTROL
    ILI9341_write_command(0xB1);
    ILI9341_write_data(0x00);
    ILI9341_write_data(0x18);

    // DISPLAY FUNCTION CONTROL
    ILI9341_write_command(0xB6);
    ILI9341_write_data(0x08);
    ILI9341_write_data(0x82);
    ILI9341_write_data(0x27);

    // 3GAMMA FUNCTION DISABLE
    ILI9341_write_command(0xF2);
    ILI9341_write_data(0x00);

    // GAMMA CURVE SELECTED
    ILI9341_write_command(0x26);
    ILI9341_write_data(0x01);

    // POSITIVE GAMMA CORRECTION
    ILI9341_write_command(0xE0);
    ILI9341_write_data(0x0F);
    ILI9341_write_data(0x31);
    ILI9341_write_data(0x2B);
    ILI9341_write_data(0x0C);
    ILI9341_write_data(0x0E);
    ILI9341_write_data(0x08);
    ILI9341_write_data(0x4E);
    ILI9341_write_data(0xF1);
    ILI9341_write_data(0x37);
    ILI9341_write_data(0x07);
    ILI9341_write_data(0x10);
    ILI9341_write_data(0x03);
    ILI9341_write_data(0x0E);
    ILI9341_write_data(0x09);
    ILI9341_write_data(0x00);

    // NEGATIVE GAMMA CORRECTION
    ILI9341_write_command(0xE1);
    ILI9341_write_data(0x00);
    ILI9341_write_data(0x0E);
    ILI9341_write_data(0x14);
    ILI9341_write_data(0x03);
    ILI9341_write_data(0x11);
    ILI9341_write_data(0x07);
    ILI9341_write_data(0x31);
    ILI9341_write_data(0xC1);
    ILI9341_write_data(0x48);
    ILI9341_write_data(0x08);
    ILI9341_write_data(0x0F);
    ILI9341_write_data(0x0C);
    ILI9341_write_data(0x31);
    ILI9341_write_data(0x36);
    ILI9341_write_data(0x0F);

    // EXIT SLEEP
    ILI9341_write_command(0x11);
    HAL_Delay(120);

    // TURN ON DISPLAY
    ILI9341_write_command(0x29);

    // MEMORY WRITE
    ILI9341_write_command(0x2C);
}

void ILI9341_set_address(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2) {
    // Ustawienie kolumn (oś X)
    ILI9341_write_command(0x2A);
    ILI9341_write_data(X1 >> 8);    // górny bajt X1
    ILI9341_write_data(X1 & 0xFF);  // dolny bajt X1
    ILI9341_write_data(X2 >> 8);    // górny bajt X2
    ILI9341_write_data(X2 & 0xFF);  // dolny bajt X2

    // Ustawienie wierszy (oś Y)
    ILI9341_write_command(0x2B);
    ILI9341_write_data(Y1 >> 8);    // górny bajt Y1
    ILI9341_write_data(Y1 & 0xFF);  // dolny bajt Y1
    ILI9341_write_data(Y2 >> 8);    // górny bajt Y2
    ILI9341_write_data(Y2 & 0xFF);  // dolny bajt Y2

    // Zapisz do ramu
    ILI9341_write_command(0x2C);
}
