#ifndef I2C_H
#define I2C_H

#include <stdint.h> // Para usar tipos como uint8_t

// --- Prottipos de Funes ---

/**
 * @brief Inicializa o hardware de I2C (TWI - Two-Wire Interface) do ATmega328P.
 * Configura a taxa de bits para 100kHz (padro).
 */
void I2C_Init(void);

/**
 * @brief Envia uma condio de START no barramento I2C.
 */
void I2C_Start(void);

/**
 * @brief Envia uma condio de STOP no barramento I2C.
 */
void I2C_Stop(void);

/**
 * @brief Escreve um byte de dados no barramento I2C.
 * @param data O byte a ser enviado.
 * @return 0 se o slave deu ACK, 1 se deu NACK.
 */
uint8_t I2C_Write(uint8_t data);

#endif // I2C_H