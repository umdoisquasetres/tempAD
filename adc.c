#include <xc.h>
#include "adc.h"

void ADC_Init(void) {
    // ADCON1: Configura pinos analógicos e VRef
    // PCFG<3:0> = 1101: AN0 e AN1 são analógicos, os demais são digitais.
    // VCFG<1:0> = 00: Tensão de referência VSS e VDD.
    ADCON1 = 0x0D; 

    // ADCON2: Configura justificação, tempo de aquisição e clock do ADC
    // ADFM = 1: Resultado justificado à direita (bits 9:0 em ADRESH:ADRESL)
    // ACQT<2:0> = 101: 12 TAD (Tempo de Aquisição)
    // ADCS<2:0> = 010: Clock FOSC/32
    ADCON2 = 0xB5; // 0b10110101
    
    // ADCON0: Habilita o módulo ADC
    ADCON0bits.ADON = 1;
}

unsigned int ADC_Read(unsigned char channel) {
    // 1. Seleciona o canal
    // Limpa os bits do canal (CHS<3:0>) e mantém ADON ligado
    ADCON0 = (ADCON0 & 0xC3) | ((channel << 2) & 0x3C);

    // 2. Inicia a conversão
    ADCON0bits.GO_DONE = 1;

    // 3. Espera a conversão terminar
    while (ADCON0bits.GO_DONE);

    // 4. Retorna o resultado (10 bits)
    return ((ADRESH << 8) | ADRESL);
}