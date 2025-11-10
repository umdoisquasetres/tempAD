
#ifndef I2C_H
#define I2C_H

#include <stdint.h> // Para usar tipos como uint8_t

// --- Prot�tipos de Fun��es ---

/**
 * @brief Inicializa o hardware de I2C (TWI - Two-Wire Interface) do ATmega328P.
 * Configura a taxa de bits para 100kHz (padr�o).
 */
void I2C_Init(void);

/**
 * @brief Envia uma condi��o de START no barramento I2C.
 */
void I2C_Start(void);

/**
 * @brief Envia uma condi��o de STOP no barramento I2C.
 */
void I2C_Stop(void);

/**
 * @brief Escreve um byte de dados no barramento I2C.
 * @param data O byte a ser enviado.
 * @return 0 se o slave deu ACK, 1 se deu NACK.
 */
uint8_t I2C_Write(uint8_t data);

/**
 * @brief L� um byte do barramento I2C e envia um ACK (Acknowledge).
 * Usado para continuar recebendo mais bytes.
 * @return O byte lido.
 */
uint8_t I2C_Read_ACK(void);

/**
 * @brief L� um byte do barramento I2C e envia um NACK (Not Acknowledge).
 * Usado para sinalizar o fim da recep��o de dados.
 * @return O byte lido.
 */
uint8_t I2C_Read_NACK(void);

#endif // I2C_H
