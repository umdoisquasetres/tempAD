#include <xc.h>
#include "i2c.h"


// --- Funes Auxiliares Internas ---

// Espera o barramento I2C ficar livre (idle)
void I2C_Wait(void) {
    // Espera enquanto a comunicao est em progresso
    // (SSPSTAT<2> R_W) ou (SSPCON2<4:0> ACKEN, RCEN, PEN, RSEN, SEN)
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

// --- Funes Pblicas ---

void I2C_Init(void) {
    // 1. Configurar pinos RC3 (SCL) e RC4 (SDA) como entradas
    // O mdulo MSSP assume o controle deles
    TRISCbits.TRISC3 = 1; 
    TRISCbits.TRISC4 = 1;

    // 2. Configurar o mdulo MSSP
    SSPSTAT = 0x80; // 0b10000000 - Slew Rate Control desabilitado (para 100kHz)
    SSPCON1 = 0x28; // 0b00101000 - Habilita MSSP, Modo I2C Master
    SSPCON2 = 0x00; // Limpa registradores de controle

    // 3. Configurar o Baud Rate (Velocidade)
    // Frmula: SSPADD = (FOSC / (4 * F_SCL)) - 1
    // Ex: (8,000,000 / (4 * 100,000)) - 1 = (20) - 1 = 19
    SSPADD = (_XTAL_FREQ / (4 * F_SCL)) - 1;
}

void I2C_Start(void) {
    I2C_Wait(); // Espera estar livre
    SSPCON2bits.SEN = 1; // Gera condio de START
    while (SSPCON2bits.SEN); // Espera o START completar
}

void I2C_Stop(void) {
    I2C_Wait(); // Espera estar livre
    SSPCON2bits.PEN = 1; // Gera condio de STOP
    while (SSPCON2bits.PEN); // Espera o STOP completar
}

// Retorna 0 se ACK, 1 se NACK
uint8_t I2C_Write(uint8_t data) {
    I2C_Wait(); // Espera estar livre
    SSPBUF = data; // Carrega o dado para enviar
    while (!PIR1bits.SSPIF); // Espera a transmisso (9 pulso de clock)
    PIR1bits.SSPIF = 0; // Limpa a flag de interrupo
    
    // Checa ACK (ACKSTAT=0  ACK, 1  NACK)
    if(SSPCON2bits.ACKSTAT) {
        return 1; // NACK recebido
    }
    return 0; // ACK recebido
}

uint8_t I2C_Read_ACK(void) {
    uint8_t data;
    I2C_Wait();
    SSPCON2bits.RCEN = 1; // Habilita modo de recepo
    while (!SSPSTATbits.BF); // Espera o buffer ficar cheio
    data = SSPBUF; // L o dado
    
    I2C_Wait();
    SSPCON2bits.ACKDT = 0; // Prepara para enviar ACK
    SSPCON2bits.ACKEN = 1; // Envia o ACK
    while (SSPCON2bits.ACKEN); // Espera o envio do ACK
    return data;
}

uint8_t I2C_Read_NACK(void) {
    uint8_t data;
    I2C_Wait();
    SSPCON2bits.RCEN = 1; // Habilita modo de recepo
    while (!SSPSTATbits.BF); // Espera o buffer ficar cheio
    data = SSPBUF; // L o dado
    
    I2C_Wait();
    SSPCON2bits.ACKDT = 1; // Prepara para enviar NACK
    SSPCON2bits.ACKEN = 1; // Envia o NACK
    while (SSPCON2bits.ACKEN); // Espera o envio do NACK
    return data;
}
