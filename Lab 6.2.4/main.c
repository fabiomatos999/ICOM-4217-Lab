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

input fullstep[4] = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } }; // define an array of input structures called fullstep with 4 elements, each element containing values for a, b, abar, and bbar
input halfstep[8] = { { 1, 0, 0, 0 }, { 1, 1, 0, 0 }, { 0, 1, 0, 0 }, { 0, 1, 1, 0 },
                      { 0, 0, 1, 0 }, { 0, 0, 1, 1 }, { 0, 0, 0, 1 }, { 1, 0, 0, 1 } }; // define an array of input structures called halfstep with 8 elements, each element containing values for a, b, abar, and bbar

int currstep = 0;
unsigned int degree = 0;
direction currdirr = NONE;
unsigned int stepcount = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    TA0CTL = TASSEL_1 + MC_1; // configure timer A0 with ACLK as the source and up mode
    TA0CCTL0 = CCIE; // enable timer A0 interrupt
    TA0CCR0 = 32768 / 100; // set timer A0 CCR0 value to generate an interrupt every 10 ms
    P10DIR = 0x0F; // configure port 10 pins 0-3 as outputs

    __enable_interrupt();
    moveDeg(270, CCW); // move the stepper motor 270 degrees counterclockwise
    moveDeg(180, CCW); // move the stepper motor 180 degrees counterclockwise
    moveDeg(180, CW); // move the stepper motor 180 degrees counterclockwise
    while(1){}
}

// function that moves the stepper motor one full step in the specified direction

void moveFullStep(direction dir) 
{
    volatile int step;
    P10OUT = ((fullstep[currstep].a << 0) + (fullstep[currstep].b << 1) 
            + (fullstep[currstep].abar << 2) + (fullstep[currstep].bbar << 3)); // set the output values of the stepper motor driver based on the current step

            /* a, b, abar, and bbar are input signals to the stepper motor driver
            that determine the direction of rotation and the number of steps taken
            based on their combination. */


    if (dir == CW) // if the direction input is CW (clockwise)
    {
        currstep = (++currstep) % 4; // increment the current step by 1 and wrap around to 0 if it exceeds 3
    }
    else if (dir == CCW) // if the direction input is CCW (counterclockwise)
    {
        currstep -= 1; 
        if (currstep < 0){
            currstep = 3; // decrement the current step by 1 set the current step to 3 (wrap around to the last step)
        }
    }
    return;

}

// This function moves the stepper motor in half-step mode in the specified direction based on the current step position and direction.

void moveHalfStep(direction dir)
{

    // Set the output pins of the stepper motor driver to the values of the current half-step
    P10OUT = ((halfstep[currstep].a << 0) + (halfstep[currstep].b << 1)
            + (halfstep[currstep].abar << 2) + (halfstep[currstep].bbar << 3));

    // Update the current step position based on the direction
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

// This is an interrupt service routine that is called when Timer A0 triggers
// It controls the stepper motor by executing full steps at a specified rate

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    volatile int a;
    a = degree/5.625; // Calculate the number of steps required to move the motor by the specified degree angle

    // If the number of steps executed is greater than or equal to the number of steps required to move the motor by the specified angle, reset the step count and current direction
    if(stepcount > degree/(5.625/8))
    {
        stepcount = 0;
        currdirr = NONE;
    }

    // If the current direction is not NONE, increment the step count and execute a full step in the current direction
    if (currdirr != NONE)
    {
        stepcount += 1;
        moveFullStep(currdirr);
    }
}


// This function sets the current direction and degree angle for the stepper motor movement. It waits until the movement is completed before returning
void moveDeg(unsigned int deg, direction dir)
{
    currdirr = dir;
    degree = deg;
    while (currdirr != NONE){}
}
