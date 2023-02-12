#include <msp430.h>
void main()
{

    P1DIR &= ~0x70; // P1.4 = Column 1, P1.5 = Column 2, P1.6 = Column 3
    P1REN |= 0x70;
    P1IFG &= ~0x70;
    P1IES |= 0x70;
    P1IE |= 0x70;
    P4DIR |= 0x0F; // P4.0 = Row 1, P4.1 = Row 2, P4.2 = Row 3, P4.3 = Row 4
    P4OUT |= 0x0F;
    __enable_interrupt();
    while (1){
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1()
{
    unsigned volatile int column1 = (P1IFG >> 4) & 1;
    unsigned volatile int column2 = (P1IFG >> 5) & 1;
    unsigned volatile int column3 = (P1IFG >> 6) & 1;
    P1IFG &= ~0x70;
}
