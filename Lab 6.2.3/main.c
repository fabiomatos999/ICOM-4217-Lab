#include <msp430.h> 


/**
 * main.c
 */

float angles[] = {-67.5,22.5,0,45,-45,90,67.5,-22.5,0};
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

    send_to_degree(angles[angle]);

    TB0CTL |= 0x110;
    TB0CCTL0 |= 0x10;
    TB0CCR0 = 0x8000;
    __enable_interrupt();
    _bis_SR_register(LPM0_bits);
}

void send_to_degree(float deg){
    if(deg < -90 && deg > 90){return;}
    volatile float dc = ((deg/180.0)+(3.0/2.0))/20;
    TA0CCR1 = dc*TA0CCR0;
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B(void){
    angle = (++angle)%9;
    unsigned volatile int uwu = angle;
    send_to_degree(angles[angle]);
}
