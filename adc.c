/* 
 * File:   adc.c
 * Author: Karthic Kumar
 *
 * Created on February 25, 2017, 11:36 AM
 */
#include "p33FJ16GS502.h"

int 		ADC_RSLT0=0;
int 		ADC_RSLT1=0;
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void __attribute__((__interrupt__,no_auto_psv)) _ADCP0Interrupt ()
 {	IFS6bits.ADCP0IF=0;
	ADC_RSLT0 = ADCBUF0; 			// Read AN0 conversion result
	ADC_RSLT1 = ADCBUF1; 			// Read AN1 conversion result 
	ADSTATbits.P0RDY = 0; 				// Clear the data is ready in buffer bits
	LATBbits.LATB4^=1;				// Toggle B4 pin output
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void init_ADC(void)  
{	 
    	ADCONbits.FORM = 1;			//Integer data format
    	ADCONbits.EIE = 1;				//Early Interrupt disabled
    	ADCONbits.ORDER = 0;			//Convert even channel first
    	ADCONbits.SEQSAMP = 0;		//Select simultaneous sampling
    	ADCONbits.ADCS = 5;			//ADC clock = FADC/6 = 120MHz / 6 = 20MHz

    	ADPCFGbits.PCFG0 = 0; 		 //select CH0 as analog input pin 
    	ADPCFGbits.PCFG1 = 0; 		 //select CH1 as analog input pin

    	IFS6bits.ADCP0IF = 0;			//Clear ADC Pair 0 interrupt flag 
    	IPC27bits.ADCP0IP = 5;			//Set ADC Pair 0 interrupt priority
    	IEC6bits.ADCP0IE = 1;			//Enable the ADC Pair 0 interrupt

     	ADSTATbits.P0RDY = 0; 			//Clear Pair 0 data ready bit
    	ADCPC0bits.IRQEN0 = 1;		//Enable ADC Interrupt pair 0 
    	ADCPC0bits.TRGSRC0 = 4; 		//ADC Pair 0 triggered by PWM1 Trigger

	TRGCON1bits.DTM=1;			//dual trigger mode
	TRIG1bits.TRGCMP=0;			//Primary trig compare value 
	STRIG1bits.STRGCMP=0xFFF;	//secondary trig compare value

 	TRGCON1bits.TRGDIV = 0;		// Trigger generated every PWM cycle
	TRGCON1bits.TRGSTRT = 0;		// enable Trigger generated after 0 PWM cycles 
	TRIG1 = 1;						// Trigger compare value
}
