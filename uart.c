/* 
 * File:   uart.c
 * Author: Karthic Kumar
 *
 * Created on February 25, 2017, 11:36 AM
 */

//Fcy = (Fosc/2); Fosc = 80MHz
//#define FCY 40000000ULL
//#define FCY   39920833ULL
#define FCY   32134375ULL
#define BAUDRATE 4000000
//#define BRGVAL ((FCY/BAUDRATE)/4)-1
#define BRGVAL 1

#include "p33FJ16GS502.h"
#include <libpic30.h>


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

U1BRG = BRGVAL;         // Baud Rate setting 
U1STAbits.UTXISEL0 = 0;
U1STAbits.UTXISEL1 = 0; // Interrupt after one TX character is transmitted
U1STA = 0;


IEC0bits.U1TXIE = 1;    // Enable UART TX interrupt
U1MODEbits.UARTEN = 1;  // Enable UART
U1STAbits.UTXEN = 1;    // Enable UART TX
//Rx on RP4
RPINR18bits.U1RXR = 4;
//TX on RP3
RPOR1bits.RP3R = 3;

}



