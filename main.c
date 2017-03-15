/* 
 * File:   main.c
 * Author: Karthic Kumar
 *
 * Created on February 25, 2017, 11:36 AM
 */

// DSPIC33FJ16GS502 Configuration Bit Settings

// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (General Segment may be written)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = FRCPLL           // Oscillator Source Selection (Internal Fast RC (FRC) with PLL)
#pragma config IESO = OFF               // Internal External Switch Over Mode (Start up device with user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Source (Primary oscillator disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function (OSC2 is general purpose digital I/O pin)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled, Mon Disabled)

// FWDT
#pragma config WDTPOST = PS1            // Watchdog Timer Postscaler (1:1)
#pragma config WDTPRE = PR32            // WDT Prescaler (1:32)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is disabled)

#define DELAY_105uS asm volatile ("REPEAT, #4201"); Nop();  // 105uS delay

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//#define FCY 40000000ULL
//#define FCY   39920833ULL
#define FCY   32134375ULL

#include "p33FJ16GS502.h"
#include <libpic30.h>




/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void    init_UART(void);
void 	init_PWM(void);
void 	init_ADC(void);


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/											  
int main(void)
{
	// Configure PLL prescaler, PLL postscaler, PLL divisor  
    // setup internal clock for 80MHz/40MIPS
    // 7.37/3=2.456666 * 65 =159.683333 / 2 = 79.84166667 (~80MHz)
	PLLFBD=33; 							// M = PLLFBD + 2  = 35
	CLKDIVbits.PLLPOST=0;   				// N1 = 2  
	CLKDIVbits.PLLPRE=0;    				// N2 = 2  
    OSCTUN = 0x3F;                          //Oscillator tuning 
    RCONbits.SWDTEN = 0;                //disable watch dog timer in software

 // __builtin_write_OSCCONH(0x01);			// New Oscillator FRC w/ PLL  
 // __builtin_write_OSCCONL(OSCCON | 0x01);  		// Enable Switch  
      
//	while(OSCCONbits.COSC != 0b001);		// Wait for new Oscillator to become FRC w/ PLL    
    while(OSCCONbits.LOCK != 1);			// Wait for Pll to Lock  

	ACLKCONbits.FRCSEL = 1;				// FRC provides input for Auxiliary PLL (x16)  
	ACLKCONbits.SELACLK = 1;			// Auxiliary Oscillator provides clock source for PWM & ADC  
	ACLKCONbits.APSTSCLR = 7;			// Divide Auxiliary clock by 1  
	ACLKCONbits.ENAPLL = 1;				// Enable Auxiliary PLL  
	
	while(ACLKCONbits.APLLCK != 1);		// Wait for Auxiliary PLL to Lock  

    init_UART();                        // UART Setup
//	init_PWM();							// PWM Setup		
//	init_ADC();							// ADC Setup 
	
    
	TRISA = 0;		    	//Port A as output

  //  PTCONbits.PTEN = 1;					// Enable the PWM  
  //	ADCONbits.ADON = 1;					// Enable the ADC  

    /* Wait at least 105 microseconds (1/9600) before sending first char on UART */
    DELAY_105uS
    U1TXREG = 'a';  // Transmit one character
    
    while(1){
        LATA = 0xff;
        __delay_ms(100);
        LATA = 0x00;
        __delay_ms(100);
    }; 
    //test
} 





