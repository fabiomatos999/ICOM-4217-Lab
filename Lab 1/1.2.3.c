#include <msp430.h>				


/**
 * blink.c
 */
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P4DIR |= 0x01;					// configure P4.0 as output
	P4DIR &= 0x02;

	volatile unsigned int i;		// volatile to prevent optimization

	while(1)
	{
		P4
	}
}
