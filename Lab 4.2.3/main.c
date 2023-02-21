#include <msp430.h> 


/**
 * main.c
 */
unsigned int index = 0;
void display_num(unsigned int num){
    switch (num){
    case 0x0:
        P10OUT = ~0x3F;
        break;
    case 0x1:
        P10OUT = ~0x06;
        break;
    case 0x2:
        P10OUT = ~0x5B;
        break;
    case 0x3:
        P10OUT = ~0x4F;
        break;
    case 0x4:
        P10OUT = ~0x66;
        break;
    case 0x5:
        P10OUT = ~0x6D;
        break;
    case 0x06:
        P10OUT = ~0x7D;
        break;
    case 0x7:
        P10OUT = ~0x07;
        break;
    case 0x8:
        P10OUT = ~0x7F;
        break;
    case 0x9:
        P10OUT = ~0x6F;
        break;
    case 0xA:
        P10OUT = ~0x77;
        break;
    case 0xB:
        P10OUT = ~0x7C;
        break;
    case 0xC:
        P10OUT = ~0x39;
        break;
    case 0xD:
        P10OUT = ~0x5E;
        break;
    case 0xE:
        P10OUT = ~0x79;
        break;
    case 0xF:
        P10OUT = ~0x71;
        break;
    }
}
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog time
    TA0CTL |= 0x110; // Setting Timer A0 using ACLK with 1 prescaler
    TA0CCTL0 |= 0x10;
    TA0CCR0 = 0x8000;
    P1DIR = 0x80;
    P1OUT = 0x00;
    P10DIR = 0xFF;
    display_num(0);
    __enable_interrupt();
    _BIS_SR(LPM0_bits);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){
    display_num(index);
    index = (++index)%16;
}




