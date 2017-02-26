/* 
 * File:   pwm.c
 * Author: Karthic Kumar
 *
 * Created on February 25, 2017, 11:36 AM
 */

#include "p33FJ16GS502.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void init_PWM(void)    
{
 	PTCON2bits.PCLKDIV=0b110;		//PWM input clk devider =64
	PHASE1 = 0xFFFF;					// Timer1 period=max, this is just convenient for validation 
	PDC1 = 0x7FFF; 					// Duty Cycle is 50%  
	IOCON1bits.PENH = 1;                  	// PWM1H is controlled by PWM module
	IOCON1bits.PENL = 1;                  	// PWM1H is controlled by PWM module	
	IOCON1bits.POLH = 0;                  	// Drive signals are active-high 
	IOCON1bits.PMOD=1;				// PWM pair works in Independent Mode
	PWMCON1bits.ITB = 1;				// Select Independent Timebase mode
}
