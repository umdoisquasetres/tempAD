#include <xc.h>
#include "adc.h"

void ADC_Init(void) {
    // ADCON1: Configura pinos anal�gicos e VRef
    // PCFG<3:0> = 1101: AN0 e AN1 s�o anal�gicos, os demais s�o digitais.
    // VCFG<1:0> = 00: Tens�o de refer�ncia VSS e VDD.
    ADCON1 = 0x0D; 

    // ADCON2: Configura justifica��o, tempo de aquisi��o e clock do ADC
    // ADFM = 1: Resultado justificado � direita (bits 9:0 em ADRESH:ADRESL)
    // ACQT<2:0> = 101: 12 TAD (Tempo de Aquisi��o)
    // ADCS<2:0> = 010: Clock FOSC/32
    ADCON2 = 0xB5; // 0b10110101
    
    // ADCON0: Habilita o m�dulo ADC
    ADCON0bits.ADON = 1;
}

unsigned int ADC_Read(unsigned char channel) {
    // 1. Seleciona o canal
    // Limpa os bits do canal (CHS<3:0>) e mant�m ADON ligado
    ADCON0 = (ADCON0 & 0xC3) | ((channel << 2) & 0x3C);

    // 2. Inicia a convers�o
    ADCON0bits.GO_DONE = 1;

    // 3. Espera a convers�o terminar
    while (ADCON0bits.GO_DONE);

    // 4. Retorna o resultado (10 bits)
    return ((unsigned int)ADRESH << 8) | (unsigned int)ADRESL;
}