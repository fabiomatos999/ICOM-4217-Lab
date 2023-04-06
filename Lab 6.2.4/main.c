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

input fullstep[4] = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
input halfstep[8] = { { 1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 1, 0 },
                      { 0, 0, 1, 0 }, { 0, 0, 1, 1 }, { 0, 0, 0, 1 }, { 1, 0, 0, 1 } };

int currstep = 0;
unsigned int degree = 0;
direction currdirr = NONE;
unsigned int stepcount = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    TA0CTL = TASSEL_1 + MC_1;
    TA0CCTL0 = CCIE;
    TA0CCR0 = 32768 / 100;
    P10DIR = 0x0F;

    __enable_interrupt();
    moveDeg(270, CCW);
    moveDeg(180, CCW);
    moveDeg(180, CW);
    while(1){}
}

void moveFullStep(direction dir)
{
    volatile int step;
    P10OUT = ((fullstep[currstep].a << 0) + (fullstep[currstep].b << 1)
            + (fullstep[currstep].abar << 2) + (fullstep[currstep].bbar << 3));
    if (dir == CW)
    {
        currstep = (++currstep) % 4;
    }
    else if (dir == CCW)
    {
        currstep -= 1;
        if (currstep < 0){
            currstep = 3;
        }
    }
    return;

}

void moveHalfStep(direction dir)
{
    P10OUT = ((halfstep[currstep].a << 0) + (halfstep[currstep].b << 1)
            + (halfstep[currstep].abar << 2) + (halfstep[currstep].bbar << 3));
    if (dir == CW)
    {
        currstep = (++currstep) % 8;
    }
    else if (dir == CCW)
    {
        currstep -= 1;
        if (currstep < 0){
            currstep = 7;
        }
    }
    return;

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    volatile int a;
    a = degree/5.625;
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
    while (currdirr != NONE){}
}
