#include <msp430.h> 


/**
 * main.c
 */
// Bit Fields: https://www.tutorialspoint.com/cprogramming/c_bit_fields.htm
#define SIZE 5
unsigned long half_periods[SIZE] = {0x20,0x10,0x11,0x09,0x06}; // Half Periods for 0.5, 1, 1.5, 2, 3 Khz respectively
unsigned int index = 0;
unsigned long half_period = 0x20;
void update_half_period(unsigned int * i,  unsigned long * half){
    *i = (++(*i))%SIZE;
    *half = half_periods[*i];
    TA0CCR0 = *half;
}
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	TA0CTL |= 0x110; // Setting Timer A0 using ACLK with 1 prescaler
	TA0CCTL0 |= 0x10;
	TA0CCR0 = half_period;
	P1DIR |= 0x04;
	P1OUT |= 0x04;
	P1REN |= 0x10;
	P1IFG &= ~0x10;
	P1IES |= 0x10;
	P1IE |= 0x10;
    __enable_interrupt();
	
    _BIS_SR(LPM4_bits);
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){
    P1OUT ^= 0x04;
}


#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(){
    update_half_period(&index,&half_period);
    __delay_cycles(250000);
    P1IFG &= ~0x10;
}
