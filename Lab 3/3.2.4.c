#include "display.h"
#include <msp430.h>
char keypad_keys[12] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0',
                         '#' };
void main()
{
    init();
    P1DIR &= ~0x70; // P1.4 = Column 1, P1.5 = Column 2, P1.6 = Column 3r
    P1REN |= 0x70;
    P1IFG &= ~0x70;
    P1IES |= 0x70;
    P1IE |= 0x70;
    P10DIR |= 0x0F; // P4.0 = Row 1, P4.1 = Row 2, P4.2 = Row 3, P4.3 = Row 4
    P10OUT = 0x01;
    __enable_interrupt();
    while (1)
    {
        P10OUT = P10OUT << 1;
        __delay_cycles(4000);
        P10OUT = P10OUT << 1;
        __delay_cycles(4000);
        P10OUT = P10OUT << 1;
        __delay_cycles(4000);
        P10OUT = P10OUT >> 3;
        __delay_cycles(4000);

    }
}

char decode_pad(unsigned volatile int column, int port)
{
    char decoded_char = NULL;
    if (column == 1)
    {
        switch (port)
        {
        case 0x04:
            decoded_char = keypad_keys[0];
            break;
        case 0x08:
            decoded_char = keypad_keys[3];
            break;
        case 0x01:
            decoded_char = keypad_keys[6];
            break;
        case 0x02:
            decoded_char = keypad_keys[9];
        }

    }
    return decoded_char;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1()
{
    unsigned volatile int column1 = (P1IFG >> 4) & 1;
    unsigned volatile int column2 = (P1IFG >> 5) & 1;
    unsigned volatile int column3 = (P1IFG >> 6) & 1;
    unsigned volatile int column = 0;
    if (column1)
    {
        column = 1;
    }
    else if (column2)
    {
        column = 2;
    }
    else if (column3)
    {
        column = 3;
    }
    else
    {
        return;
    }
    P1IFG &= ~0x70;
}
