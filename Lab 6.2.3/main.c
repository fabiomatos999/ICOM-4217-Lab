#include <msp430.h> 


/**
 * main.c
 */

float angles[] = {-67.5,22.5,0,45,-45,90,67.5,-22.5,-90};
unsigned int angle = 0;

void send_to_degree(float deg);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    //Timer A0 Setup
    TA0CTL |= TASSEL_2 + MC_1;
    TA0CCR0 = 1048576.0/50.0; // 50 Hz

    //Timer A0 CC1 Setup (RED)
    P1DIR |= BIT2;
    P1SEL |= BIT2;
    TA0CCTL1 |= OUTMOD_7;

    TA1CTL |= 0x110;
    TA1CCTL0 |= 0x10;
    TA1CCR0 = 0xFF00;
    __enable_interrupt();
    send_to_degree(0);
    while(1){

    }
}

void send_to_degree(float deg){
    if(deg < -90 && deg > 90){return;}
    volatile float dc = ((deg/90.0)+(1.5))/20.0;
    TA0CCR1 = dc*TA0CCR0;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A1(void){
    send_to_degree(angles[angle]);
    angle = (++angle)%9;
}
