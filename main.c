// PIC18F4685 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = IRCIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (VBOR set to 2.1V)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config BBSIZ = 1024     // Boot Block Size Select bits (1K words (2K bytes) Boot Block)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)
#pragma config CP4 = OFF        // Code Protection bit (Block 4 (010000-013FFFh) not code-protected)
#pragma config CP5 = OFF        // Code Protection bit (Block 5 (014000-017FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)
#pragma config WRT4 = OFF       // Write Protection bit (Block 4 (010000-013FFFh) not write-protected)
#pragma config WRT5 = OFF       // Write Protection bit (Block 5 (014000-017FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not code-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR4 = OFF      // Table Read Protection bit (Block 4 (010000-013FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR5 = OFF      // Table Read Protection bit (Block 5 (014000-017FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ 8000000

// --- Incluso de Bibliotecas ---
#include <xc.h>
#include <math.h>
#include <stdio.h> 
#include <string.h>
// #include <util/delay.h> // REMOVIDO

// --- Incluso dos nossos drivers ---
#include "lcd_i2c.h"
#include "adc.h"

// ... (Pinos, Constantes do Termmetro, etc. - TUDO IGUAL) ...
#define SENSOR_CHANNEL 0 // AN0
#define REF_CHANNEL    1 // AN1

#define R_FIXO 10000.0
#define STEINHART_A 0.001129148
#define STEINHART_B 0.000234125
#define STEINHART_C 0.0000000876741
#define ADC_MAX    1023.0 
#define N_AMOSTRAS 10     

// --- Funo ftoa (permanece igual) ---
void ftoa(float f, char *buffer, int precision) {
    int integerPart = (int)f;
    float fractionalPart = f - (float)integerPart;
    
    sprintf(buffer, "%d.", integerPart);
    
    if (precision > 0) {
        for (int i = 0; i < precision; i++) {
            fractionalPart *= 10;
        }
        int fractionalAsInt = (int)fabs(fractionalPart);
        
        char format[10];
        sprintf(format, "%%0%dd", precision); 
        sprintf(buffer + strlen(buffer), format, fractionalAsInt);
    }
}

int main(void) {
    // --- Configurao do Oscilador Interno ---
    // OSCCON = 0x72: 8MHz, estvel, fonte interna
    OSCCON = 0x70; 

    // --- Inicializao dos Mdulos ---
    ADC_Init();  
    LCD_Init();  

    LCD_PrintString("YSS Termometro...");
    LCD_SetCursor(0, 1);
    LCD_PrintString("Iniciando PIC...");
    __delay_ms(500); // MUDANA AQUI
    LCD_Clear();

    // --- Loop Principal ---
    while (1) {
        long leituraSensorSum = 0;
        long leituraRefSum = 0;
        int leituraSensorAvg;
        int leituraRefAvg;
        float resistenciaNTC;
        float tempKelvin;
        float tempCelsius;
        
        char tempBuffer[16]; 
        char tensaoBuffer[16]; 

        // 1. Ler os dois ramos
        for (int i = 0; i < N_AMOSTRAS; i++) {
            leituraSensorSum += ADC_Read(SENSOR_CHANNEL); // AN0
            leituraRefSum += ADC_Read(REF_CHANNEL);     // AN1
            __delay_ms(2); // MUDANA AQUI
        }
        leituraSensorAvg = leituraSensorSum / N_AMOSTRAS;
        leituraRefAvg = leituraRefSum / N_AMOSTRAS;

        // ... (Clculo da resistnciaNTC - TUDO IGUAL) ...
        if (leituraSensorAvg <= 0 || leituraSensorAvg >= ADC_MAX ||
            leituraRefAvg <= 0 || leituraRefAvg >= ADC_MAX ||
            (2.0 * leituraRefAvg - leituraSensorAvg) <= 0) {
            LCD_SetCursor(0, 0);
            LCD_PrintString("Falha no sensor!");
            LCD_SetCursor(0, 1);
            LCD_PrintString("Verifique conexao");
            __delay_ms(1000); // MUDANA AQUI
            continue; 
        }

        resistenciaNTC = (R_FIXO * leituraSensorAvg) / (2.0 * leituraRefAvg - leituraSensorAvg);

        // ... (Clculo da temperatura - TUDO IGUAL) ...
        float logNTC = log(resistenciaNTC);
        tempKelvin = (1.0 / (STEINHART_A + (STEINHART_B * logNTC) + (STEINHART_C * logNTC * logNTC * logNTC)));
        tempCelsius = tempKelvin - 273.15;

        // ... (Exibio dos valores - TUDO IGUAL) ...
        ftoa(tempCelsius, tempBuffer, 2); 

        LCD_SetCursor(0, 0);
        LCD_PrintString("Temp: ");
        LCD_PrintString(tempBuffer);
        LCD_PrintString(" C   ");

        long valorPonte = leituraSensorAvg - leituraRefAvg;
        float tensaoPonte = (valorPonte / ADC_MAX) * 5.0;
        
        ftoa(tensaoPonte, tensaoBuffer, 2);

        LCD_SetCursor(0, 1);
        LCD_PrintString("A-B(V):");
        LCD_PrintString(tensaoBuffer);
        LCD_PrintString(" V   ");

        __delay_ms(5000); // MUDANA AQUI
    }
    
    return 0; 
}