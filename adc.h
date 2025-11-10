
#ifndef ADC_H
#define ADC_H

// --- Protótipos de Funções ---

/**
 * @brief Inicializa o módulo ADC.
 * Configura a tensão de referência para AVcc e o prescaler do clock do ADC.
 */
void ADC_Init(void);

/**
 * @brief Lê o valor de um canal específico do ADC.
 * @param channel O canal do ADC a ser lido (0-7 para A0-A7).
 * @return O valor digital de 10 bits (0-1023) lido pelo ADC.
 */
unsigned int ADC_Read(unsigned char channel);

#endif // ADC_H
