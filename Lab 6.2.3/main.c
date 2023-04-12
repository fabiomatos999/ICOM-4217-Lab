#include <msp430.h> 


/**
 * main.c
 */

float angles[] = {-67.5,22.5,0,45,-45,90,67.5,-22.5,-90}; // Declare and initialize an array of angles
unsigned int angle = 0; // Declare and initialize a variable for the current angle

void send_to_degree(float deg);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    //Timer A0 Setup
    TA0CTL |= TASSEL_2 + MC_1; // Select SMCLK as the clock source and put the timer into Up mode
    TA0CCR0 = 1048576.0/50.0;  // Set the value in TA0CCR0 to achieve a frequency of 50Hz

    //Timer A0 CC1 Setup (RED)
    P1DIR |= BIT2; // Set P1.2 as an output
    P1SEL |= BIT2; // Select Timer A0 CC1 output for P1.2
    TA0CCTL1 |= OUTMOD_7; // Set the "Reset/Set" output mode for Timer A0 CC1

    TA1CTL |= 0x110; // Set the clock source to SMCLK, put the timer into Up mode, and enable the interrupt
    TA1CCTL0 |= 0x10; // Enable interrupt for Timer A1 Capture/Compare 0
    TA1CCR0 = 0xFF00; // Set the value in TA1CCR0 to trigger the interrupt every 65536 cycles of the SMCLK
    __enable_interrupt(); // Enable interrupts globally
    send_to_degree(0); // Set the servo motor to the neutral position
    while(1){

    }
}

// Function to set the angle of the servo motor
void send_to_degree(float deg){
    if(deg < -90 && deg > 90){return;} // Check if the angle is within the valid range
    volatile float dc = ((deg/90.0)+(1.5))/20.0; // Calculate the duty cycle
    TA0CCR1 = dc*TA0CCR0; // Set the duty cycle for Timer A0 CC1
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A1(void){
    send_to_degree(angles[angle]); // Set the angle of the servo motor
    angle = (++angle)%9; // Increment the angle index and wrap around if necessary
}
