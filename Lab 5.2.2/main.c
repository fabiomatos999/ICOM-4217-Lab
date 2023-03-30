#include <msp430.h> 

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR |= BIT2; // P1.2 as output
	P1IES |= BIT3;  /// P1.3 as input with high to low transition, falling edge
	P1REN  |= BIT3; //Enables Pull up resistor
	P1IFG &= ~BIT3; // clears flag
	P1IE |= BIT3;
	__enable_interrupt(); // enables interrupt for P1.3
    P1SEL |= BIT2; // Sets P1.2 as timmer A0 for PWM output

    //Timer A0 Setup to use SMCLK to count value on TA0CCR0
    TA0CTL |= TASSEL_2 + MC_1;
    TA0CCTL1 |= OUTMOD_7;
//    TA0CCR0 = 1000; // 1KHz
//    TA0CCR0 = 1000*2; // 500Hz
//    TA0CCR0 = 1000/2; // 2 KHz
    TA0CCR0 = 1000/4; // 4 KHz
//    TA0CCR0 = 1000/8; // 8 KHz
    TA0CCR1 = TA0CCR0/2; // This sets the PMW duty cycle to 50%

    _bis_SR_register(LPM0_bits); //Low power mode 0, waiting for interrupts

}

// Interrupt service rutine that increments the value of TA0CCR1 by 100, if it passes 1100, it resets to 0.
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(){
    TA0CCR1 += 100;
    TA0CCR1 %= 1100;
    __delay_cycles(250000);
    P1IFG &= ~BIT3; // clears the flag
}
