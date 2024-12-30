/**
 * @file main.c
 * @author Seb Madgwick
 * @brief Main file.
 *
 * Device:
 * PIC10F322
 *
 * Compiler:
 * XC8 v2.46
 */

//------------------------------------------------------------------------------
// Configuration bits

#pragma config FOSC = INTOSC    // Oscillator Selection bits (INTOSC oscillator: CLKIN function disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config WDTE = SWDTEN    // Watchdog Timer Enable (WDT controlled by the SWDTEN bit in the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)
#pragma config LPBOR = ON       // Brown-out Reset Selection bits (BOR enabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)

//------------------------------------------------------------------------------
// Includes

#include "Delay.h"
#include <stdbool.h>
#include "UartTX.h"
#include <xc.h>

//------------------------------------------------------------------------------
// Functions

void main(void) {

    // Select 16 MHz oscillator
    OSCCONbits.IRCF = 0b111;

    // Configure TX output
    ANSELAbits.ANSA1 = 0;
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA1 = 1;

    // Configure digital input
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA2 = 0;
    OPTION_REGbits.nWPUEN = 0;
    WPUAbits.WPUA0 = 1;
    WPUAbits.WPUA2 = 1;
    WPUAbits.WPUA3 = 1;

    // Print firmware version
    Delay(DelayPeriod512ms);
    UartTXString("x-IMU3-SA-D3 v1.0.2\n");

    // Main program loop
    while (true) {

        // Wait for rising/falling edge
        char currentState = PORTA;
        static char previousState;
        if (previousState == currentState) {
            continue;
        }
        previousState = currentState;

        // Write message
        UartTXByte((currentState & 0b0001) != 0 ? '1' : '0'); // A0
        UartTXByte(',');
        UartTXByte((currentState & 0b1000) != 0 ? '1' : '0'); // A3
        UartTXByte(',');
        UartTXByte((currentState & 0b0100) != 0 ? '1' : '0'); // A2
        UartTXByte('\n');
    }
}

//------------------------------------------------------------------------------
// End of file
