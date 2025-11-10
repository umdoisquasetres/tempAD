#include <xc.h>
#include "i2c.h"
#include "lcd_i2c.h"

#define LCD_I2C_ADDRESS (0x27 << 1)

// Mapeamento de pinos do I2C
#define PIN_RS    (1 << 0) // Register Select
#define PIN_RW    (1 << 1) // Read/Write (geralmente aterrado, n�o usamos)
#define PIN_EN    (1 << 2) // Enable
#define PIN_BKLT  (1 << 3) // Backlight (Luz de fundo)
#define PIN_D4    (1 << 4)
#define PIN_D5    (1 << 5)
#define PIN_D6    (1 << 6)
#define PIN_D7    (1 << 7)

static uint8_t backlight_status = PIN_BKLT;

// --- Fun��es Internas ---

void LCD_Write_I2C(uint8_t data) {
    I2C_Start();
    if (I2C_Write(LCD_I2C_ADDRESS)) { // Se NACK (retorno 1)
        I2C_Stop();
        return;
    }
    if (I2C_Write(data | backlight_status)) { // Se NACK
        I2C_Stop();
        return;
    }
    I2C_Stop();
}

void LCD_Pulse_EN(uint8_t data) {
    LCD_Write_I2C(data | PIN_EN);
    __delay_us(1); // MUDAN�A AQUI
    LCD_Write_I2C(data & ~PIN_EN);
    __delay_us(50); // MUDAN�A AQUI
}

void LCD_Write_Nibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = (nibble & 0xF0) | rs;
    LCD_Write_I2C(data);
    LCD_Pulse_EN(data);
}

// --- Fun��es P�blicas ---

void LCD_SendCommand(uint8_t cmd) {
    LCD_Write_Nibble(cmd & 0xF0, 0); 
    LCD_Write_Nibble((cmd << 4) & 0xF0, 0);
}

void LCD_SendData(uint8_t data) {
    LCD_Write_Nibble(data & 0xF0, PIN_RS); 
    LCD_Write_Nibble((data << 4) & 0xF0, PIN_RS); 
}

void LCD_Init(void) {
    I2C_Init(); 

    // --- Sequ�ncia de Inicializa��o ---
    __delay_ms(40); // MUDAN�A AQUI

    LCD_Write_Nibble(0x30, 0);
    __delay_ms(5); // MUDAN�A AQUI
    LCD_Write_Nibble(0x30, 0);
    __delay_ms(1); // MUDAN�A AQUI
    LCD_Write_Nibble(0x30, 0);
    __delay_us(100); // MUDAN�A AQUI

    LCD_Write_Nibble(0x20, 0);
    __delay_us(100); // MUDAN�A AQUI

    LCD_SendCommand(0x28); 
    LCD_SendCommand(0x08); 
    LCD_SendCommand(0x01); 
    __delay_ms(2); // MUDAN�A AQUI
    LCD_SendCommand(0x06); 
    LCD_SendCommand(0x0C); 
}

void LCD_Clear(void) {
    LCD_SendCommand(0x01); 
    __delay_ms(2); // MUDAN�A AQUI
}

void LCD_SetCursor(uint8_t col, uint8_t row) {
    uint8_t address;
    if (row == 0) {
        address = 0x80 + col;
    } else {
        address = 0xC0 + col;
    }
    LCD_SendCommand(address);
}

void LCD_PrintString(const char *str) {
    while (*str) {
        LCD_SendData(*str++);
    }
}