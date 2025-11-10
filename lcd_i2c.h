
#ifndef LCD_I2C_H
#define LCD_I2C_H

#include <stdint.h>

// --- Protótipos de Funções ---

/**
 * @brief Inicializa o LCD em modo 4 bits.
 */
void LCD_Init(void);

/**
 * @brief Envia um comando para o LCD.
 * @param cmd O byte de comando.
 */
void LCD_SendCommand(uint8_t cmd);

/**
 * @brief Envia um byte de dado (caractere) para o LCD.
 * @param data O caractere a ser exibido.
 */
void LCD_SendData(uint8_t data);

/**
 * @brief Limpa a tela do LCD.
 */
void LCD_Clear(void);

/**
 * @brief Posiciona o cursor do LCD.
 * @param col Coluna (0-15).
 * @param row Linha (0-1).
 */
void LCD_SetCursor(uint8_t col, uint8_t row);

/**
 * @brief Escreve uma string de caracteres no LCD.
 * @param str Ponteiro para a string a ser exibida.
 */
void LCD_PrintString(const char *str);


#endif // LCD_I2C_H
