#include <msp430.h> 

unsigned int dacValues[] = {0x00,0x17,0x2E,0x45,0x5C,0x73,0x8A,0xA1,0xB8,0xCF,0xE6,0xFF};

unsigned int index = 0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P10DIR = 0xFF;
	
    //Timer A0 Setup
    TA0CTL |= TASSEL_1 + MC_1; // Select ACLK as the clock source and put the timer into Up mode
    TA0CCR0 = 32768;  // Set the value in TA0CCR0 to achieve a frequency of 1Hz


	return 0;
}

#pragma vector=TIMER0_A0_VECTOR // specifies that the following function is an interrupt service routine (ISR) for Timer_A
__interrupt void Timer_A(void)
{
    P10OUT = dacValues[index];
    index = (++index)%12;
}
