#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	UCA0CTL0 |= UCSYNC;
	UCA0CTL1 |= UCSSEL_2;
	UCA0BR0 = 0x6D;
	UCA0BR1 = 0x00;
	UCA0MCTL = 0x44;
	while(1){
	   while((!UCA0STAT&1)){
	       UCA0TXBUF = (unsigned int)'a';
	   }
	}
}
