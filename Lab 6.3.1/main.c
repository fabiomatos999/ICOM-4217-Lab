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

// Define two arrays of "input" type that hold different step sequences for a stepper motor
input fullstep[4] = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0,
                                                                        1 } };
input halfstep[8] = { { 1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 1,
                                                                        0 },
                      { 0, 0, 1, 0 }, { 0, 0, 1, 1 }, { 0, 0, 0, 1 }, { 1, 0, 0,
                                                                        1 } };

// Defines an array of unsigned long integers that holds different delay periods for the stepper motor
unsigned long periods[] = {104857, 52428, 20971, 10485, 5242, 2097, 1048, 524, 209, 104 };
int period = 0;

int currstep = 0;
unsigned long degree = 0;
direction currdirr = NONE;
unsigned long stepcount = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    TA0CTL = TASSEL_2 + MC_1;   // Set Timer_A clock source to SMCLK, and count up mode
    TA0CCTL0 = CCIE; // Enable Timer_A capture/compare interrupt
    TA0CCR0 = periods[period]; // Set the compare value for Timer_A to the value at index 0 of "periods" array
    P10DIR = 0x0F; // Set Port 10 pins 0-3 as output

    P1REN |= BIT4 + BIT2 + BIT5; // Enable pull-up resistor for Port 1 pins 2, 4 and 5
    P1IES |= (BIT2 + BIT4 + BIT5); // Set interrupt edge select to trigger on falling edge
    P1IFG &= ~(BIT2 + BIT4 + BIT5); // Clear Port 1 interrupt flags for pins 2, 4, and 5
    P1IE |= BIT2 + BIT4 + BIT5; // Enable interrupt for Port 1 pins 2, 4, and 5
    P10DIR |= BIT0 + BIT1; // Set direction of Port 10 pins 0 and 1 as output

    __enable_interrupt();
    currdirr = CW; // Set current direction as clockwise
    while (1)
    {
    }
}

void moveFullStep(direction dir)
{
    volatile int step;

    // sets the output of Port 10 pins 0 and 1 based on the current step in the sequence + sets the output of Port 10 pins 2 and 3 based on the current step in the sequence
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

void moveHalfStep(direction dir) // A function that takes in the direction parameter and moves the motor by half a step in the specified direction
{
    P10OUT = ((halfstep[currstep].a << 0) + (halfstep[currstep].b << 1)
            + (halfstep[currstep].abar << 2) + (halfstep[currstep].bbar << 3)); // Set the motor pins according to the current step
    if (dir == CCW)
    {
        currstep = (++currstep) % 3; // Increment the current step by 1 and set it back to 0 if it exceeds 2
    }
    else if (dir == CW)
    {
        currstep -= 1;
        if (currstep < 0) // If the current step is less than 0 (underflow)
        {
            currstep = 3; // Set the current step to the maximum step value (3)
        }
    }
    return;

}

#pragma vector=TIMER0_A0_VECTOR // specifies that the following function is an interrupt service routine (ISR) for Timer_A
__interrupt void Timer_A(void)
{
    volatile int a = degree/(5.625/8); // calculates the number of steps to rotate the stepper motor by dividing the desired degree value by the angle per step and saves it in a volatile integer variable 'a'
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

#pragma vector=PORT1_VECTOR // specifies that the following function is an ISR for Port 1
__interrupt void PORT_1()
{
    unsigned int b1 = (P1IFG >> 2) & 1; // checks if the interrupt flag for Pin 2 of Port 1 is set and stores the result in an unsigned integer variable 'b1'
    unsigned int b2 = (P1IFG >> 4) & 1; // checks if the interrupt flag for Pin 4 of Port 1 is set and stores the result in an unsigned integer variable 'b2'
    unsigned int b3 = (P1IFG >> 5) & 1; // checks if the interrupt flag for Pin 5 of Port 1 is set and stores the result in an unsigned integer variable 'b3'
    volatile int index;
    if (b1)
    {
        if (currdirr == NONE)
        {
            currdirr = CW;
            moveDeg(720,CW); // calls the 'moveDeg' function with 720 degrees and 'CW' direction as arguments
        }
        else {
            currdirr = NONE;
        }
        TA0R = 0;
        __delay_cycles(500000);

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
        __delay_cycles(500000);
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

        __delay_cycles(500000);
        P1IFG &= ~BIT5;
        TA0R = 0;
    }
    else
    {
        P1IFG &= ~(BIT2 + BIT4 + BIT5);
    }
}
