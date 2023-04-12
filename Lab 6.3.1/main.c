#include <msp430.h> 

typedef enum direction
{
    CW, CCW, NONE
} direction;

void moveFullStep(direction dir);
void moveHalfStep(direction dir);
void moveDeg(unsigned int deg, direction dir);

typedef struct input
{
    unsigned int a :1;
    unsigned int b :1;
    unsigned int abar :1;
    unsigned int bbar :1;
} input;

input fullstep[4] = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0,
                                                                        1 } };
input halfstep[8] = { { 1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 1,
                                                                        0 },
                      { 0, 0, 1, 0 }, { 0, 0, 1, 1 }, { 0, 0, 0, 1 }, { 1, 0, 0,
                                                                        1 } };
unsigned long periods[] = {104857, 52428, 20971, 10485, 5242, 2097, 1048, 524, 209, 104 };
int period = 0;

int currstep = 0;
unsigned long degree = 0;
direction currdirr = NONE;
unsigned long stepcount = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    TA0CTL = TASSEL_2 + MC_1;
    TA0CCTL0 = CCIE;
    TA0CCR0 = periods[period];
    P10DIR = 0x0F;

    P1REN |= BIT4 + BIT2 + BIT5;
    P1IES |= (BIT2 + BIT4 + BIT5);
    P1IFG &= ~(BIT2 + BIT4 + BIT5);
    P1IE |= BIT2 + BIT4 + BIT5;
    P10DIR |= BIT0 + BIT1;

    __enable_interrupt();
    currdirr = CW;
    while (1)
    {
    }
}

void moveFullStep(direction dir)
{
    volatile int step;
    P10OUT = ((fullstep[currstep].a << 0) + (fullstep[currstep].b << 1)
            + (fullstep[currstep].abar << 2) + (fullstep[currstep].bbar << 3));
    if (dir == CCW)
    {
        currstep = (++currstep) % 3;
    }
    else if (dir == CW)
    {
        currstep -= 1;
        if (currstep < 0)
        {
            currstep = 3;
        }
    }
    return;

}

void moveHalfStep(direction dir)
{
    P10OUT = ((halfstep[currstep].a << 0) + (halfstep[currstep].b << 1)
            + (halfstep[currstep].abar << 2) + (halfstep[currstep].bbar << 3));
    if (dir == CCW)
    {
        currstep = (++currstep) % 3;
    }
    else if (dir == CW)
    {
        currstep -= 1;
        if (currstep < 0)
        {
            currstep = 3;
        }
    }
    return;

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    volatile int a = degree/(5.625/8);
    volatile int b = stepcount;
    if(stepcount > degree/(5.625/8))
    {
        stepcount = 0;
        currdirr = NONE;
    }
    if (currdirr != NONE)
    {
        stepcount += 1;
        moveFullStep(currdirr);
    }
}

void moveDeg(unsigned int deg, direction dir)
{
    currdirr = dir;
    degree = deg;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1()
{
    unsigned int b1 = (P1IFG >> 2) & 1;
    unsigned int b2 = (P1IFG >> 4) & 1;
    unsigned int b3 = (P1IFG >> 5) & 1;
    volatile int index;
    if (b1)
    {
        if (currdirr == NONE)
        {
            currdirr = CW;
            moveDeg(720,CW);
        }
        else {
            currdirr = NONE;
        }
        TA0R = 0;
        __delay_cycles(100000);

        P1IFG &= ~BIT2;
    }
    else if (b2)
    {
        period +=1;
        if (period >= 6){
            period = 0;
        }
        index = period;
        TA0CCR0 = periods[period];
        __delay_cycles(100000);
        P1IFG &= ~BIT4;
        TA0R = 0;
    }
    else if (b3)
    {
        period -= 1;
        if (period < 0)
        {
            period = 6;
        }
        index = period;
        TA0CCR0 = periods[period];

        __delay_cycles(100000);
        P1IFG &= ~BIT5;
        TA0R = 0;
    }
    else
    {
        P1IFG &= ~(BIT2 + BIT4 + BIT5);
    }
}
