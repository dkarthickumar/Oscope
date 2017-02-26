/* 
 * File:   pwm.c
 * Author: Karthic Kumar
 *
 * Created on February 25, 2017, 11:36 AM
 */

#include "p33FJ16GS502.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define FP 40000000
#define BAUDRATE 3000000
#define BRGVAL ((FP/BAUDRATE)/16)-1

void __attribute__((__interrupt__,no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;    // Clear TX Interrupt flag
    U1TXREG = 'a';          // Transmit one character
}


void init_UART()
{
// Configure oscillator as needed
    
    
    
U1MODEbits.STSEL = 0;   //1-Stop bit
U1MODEbits.PDSEL = 0;   //No Parity, 8-Data bits
U1MODEbits.ABAUD = 0;   //Auto-Baud disabled
U1MODEbits.BRGH = 1;    //High-speed mode

U1BRG = BRGVAL;         // Baud Rate setting for 9600
U1STAbits.UTXISEL0 = 0;
U1STAbits.UTXISEL1 = 0; // Interrupt after one TX character is transmitted
IEC0bits.U1TXIE = 1;    // Enable UART TX interrupt
U1MODEbits.UARTEN = 1;  // Enable UART
U1STAbits.UTXEN = 1;    // Enable UART TX

RPINR18bits.U1RXR = 1;

RPOR1bits.RP2R = 3;

}



