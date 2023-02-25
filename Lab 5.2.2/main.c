#include <msp430.h> 

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1DIR |= BIT2;
	P1IES |= BIT3;
	P1REN  |= BIT3;
	P1IFG &= ~BIT3;
	P1IE |= BIT3;
	__enable_interrupt();
    P1SEL |= BIT2;
	//Timer A0 Setup
    TA0CTL |= TASSEL_2 + MC_1;
    TA0CCTL1 |= OUTMOD_7;
//    TA0CCR0 = 1000; // 1KHz
//    TA0CCR0 = 1000*2; // 500Hz
//    TA0CCR0 = 1000/2; // 2 KHz
//    TA0CCR0 = 1000/4; // 4 KHz
    TA0CCR0 = 1000/8; // 8 KHz
    TA0CCR1 = TA0CCR0/2;

    _bis_SR_register(LPM0_bits);

}
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(){
    TA0CCR1 += 100;
    TA0CCR1 %= 1100;
    __delay_cycles(250000);
    P1IFG &= ~BIT3;
}
