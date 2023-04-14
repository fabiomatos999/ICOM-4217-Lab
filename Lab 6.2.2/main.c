#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1REN |= BIT4 + BIT2 + BIT5;
    P1IES |= (BIT2 + BIT4 + BIT5);
    P1IFG &= ~(BIT2 + BIT4 + BIT5);
    P1IE |= BIT2 + BIT4 + BIT5;
    P10DIR |= BIT0 + BIT1;

    __enable_interrupt();
    _BIS_SR(LPM0_bits);

}
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1()
{
    unsigned int b1 = (P1IFG>>2)&1;
    unsigned int b2 = (P1IFG>>4)&1;
    unsigned int b3 = (P1IFG>>5)&1;
    if (b1){
        P10OUT = 0x00;
        P1IFG &= ~BIT2;
    }
    else if (b2){
        P10OUT = 0x1;
        P1IFG &= ~BIT4;
    }
    else if (b3){
        P10OUT = 0x2;
        P1IFG &= ~BIT5;
    }
    else {
        P1IFG &= ~(BIT2 + BIT4 + BIT5);
    }
}
