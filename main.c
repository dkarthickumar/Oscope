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
#pragma config FNOSC = FRCDIVN          // Oscillator Source Selection (Internal Fast RC (FRC) oscillator with postscaler)
#pragma config IESO = ON                // Internal External Switch Over Mode (Start up device with FRC, then automatically switch to user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Source (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function (OSC2 is clock output)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled, Mon Disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog timer always enabled)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is disabled)


#define DELAY_105uS asm volatile ("REPEAT, #4201"); Nop();  // 105uS delay

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "p33FJ16GS502.h"

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
    // 7.37/2=3.685*43=158.455/2=79.2275 (~80MHz)
	PLLFBD=41; 							// M = PLLFBD + 2  
	CLKDIVbits.PLLPOST=0;   				// N1 = 2  
	CLKDIVbits.PLLPRE=0;    				// N2 = 2  

    	__builtin_write_OSCCONH(0x01);			// New Oscillator FRC w/ PLL  
    	__builtin_write_OSCCONL(0x01);  		// Enable Switch  
      
	while(OSCCONbits.COSC != 0b001);		// Wait for new Oscillator to become FRC w/ PLL    
    	while(OSCCONbits.LOCK != 1);			// Wait for Pll to Lock  

	ACLKCONbits.FRCSEL = 1;				// FRC provides input for Auxiliary PLL (x16)  
	ACLKCONbits.SELACLK = 1;			// Auxiliary Ocillator provides clock source for PWM & ADC  
	ACLKCONbits.APSTSCLR = 7;			// Divide Auxiliary clock by 1  
	ACLKCONbits.ENAPLL = 1;				// Enable Auxiliary PLL  
	
	while(ACLKCONbits.APLLCK != 1);		// Wait for Auxiliary PLL to Lock  

	init_PWM();							// PWM Setup		
	init_ADC();							// ADC Setup 
	init_UART();                        // UART Setup
    
	TRISBbits.TRISB4  = 0;		    	//RB4 as output

    PTCONbits.PTEN = 1;					// Enable the PWM  
	ADCONbits.ADON = 1;					// Enable the ADC  

    /* Wait at least 105 microseconds (1/9600) before sending first char on UART */
    DELAY_105uS
    U1TXREG = 'a';  // Transmit one character
    
    while(1); 
    //test
} 





