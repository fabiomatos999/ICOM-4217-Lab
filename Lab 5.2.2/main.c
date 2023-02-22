#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    TA0CTL |= 0x110; // Setting Timer A0 using ACLK with 1 prescaler
    TA0CCTL0 |= 0x10;
    TA0CCR0 = 0x8000;
    TA0CCR1 = 0x4000;
    TA0CCTL1 |= 0xE0;
    P1DIR = 0x10;
    P1OUT = 0x10;
//    P1REN = 0x10;
    __enable_interrupt();

	
//	_BIS_SR(LPM0_bits);
    while (1){

    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){
    P1OUT ^= 0x10;
}
