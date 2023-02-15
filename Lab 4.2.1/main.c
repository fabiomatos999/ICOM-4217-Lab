#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	TA0CTL = 0x8A;
	TA0CCR0 = 0x4000;
	P1DIR = 0x04;
	P1OUT = 0x04;
    __enable_interrupt();
	
	while (1) {
	    if(TA0R >= 0x4000){
	        P1OUT ^= 0x04;
	    }
	}
}
