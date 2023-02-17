#include <msp430.h> 


/**
 * main.c
 */
// Bit Fields: https://www.tutorialspoint.com/cprogramming/c_bit_fields.htm
long half_periods[5] = {0x40,0x20,0x10,0x08,0x04};
int index = 0;
int half_period = half_periods[index];
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	TA0CTL |= 0x112;
	TA0CCR0 |= 0x20;
	P1DIR |= 0x04;
	P1OUT |= 0x04;
	P1REN |= 0x10;
	P1IFG &= ~0x10;
	P1IES |= 0x10;
	P1IE |= 0x10;
    __enable_interrupt();
	
	while (1) {
	    if(TA0R >= 0x20){
	        TA0R = 0;
	        P1OUT ^= 0x04;
	    }
	}
}
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(){

}
