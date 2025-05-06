#include "ILI9341_driver.h"

static const uint8_t* dma_image_ptr = NULL;
static uint32_t dma_image_remaining = 0;

// Flaga, czy DMA nadal działa
// 1 - działa, 0 - nie działa
static volatile uint8_t lcd_dma_busy = 0;

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
    HAL_Delay(100);

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

    // Ustaw wskaźnik w pamięci gram na takie wiersze i kolumny jak wyżej
    ILI9341_write_command(0x2C);
}

void ILI9341_draw_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t* image) {
    // Ustaw obszar rysowania
    ILI9341_set_address(x, y, width - 1, height - 1);
    // Wysyłaj dane pikseli RGB565 bajt po bajcie
    for (uint32_t i = 0; i < width * height * 2; i++) {
        ILI9341_write_data(image[i]);
    }
}

void ILI9341_draw_image_DMA(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t* image) {
    // Sprawdzenie, czy dma działa 
    if (lcd_dma_busy) return;
    lcd_dma_busy = 1;

    ILI9341_set_address(x, y, x + width - 1, y + height - 1);
    ILI9341_write_command(0x2C);

    HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

    dma_image_ptr = image;
    dma_image_remaining = width * height * 2;

    ILI9341_DMA_continue(); // start pierwszej porcji
}

void ILI9341_DMA_continue(void) {
    if (dma_image_remaining > 0) {
        // Pobranie rozmiaru porcji do wysłania jak więcej niż limit DMA to weź całość
        uint32_t chunk = (dma_image_remaining > 65535) ? 65535 : dma_image_remaining;

        HAL_SPI_Transmit_DMA(&hspi5, (uint8_t*)dma_image_ptr, chunk);
        
        // Przesunięcie wskaźnika o tyle bajtów ile zostało wysłanych
        dma_image_ptr += chunk;
        
        // Zmniejsz pozostałą liczbę bajtów do wysłania
        dma_image_remaining -= chunk;
    } else {
        // Jeśli nic nie zostało, koniec komunikacji, zwolnienie flagi
        HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
        lcd_dma_busy = 0;
    }
}
