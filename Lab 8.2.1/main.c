#include <msp430.h> 
#include <math.h>

unsigned int dacValues[] = {0x00,0x17,0x2E,0x45,0x5C,0x73,0x8A,0xA1,0xB8,0xCF,0xE6,0xFF};

int degree = 0;
unsigned int index = 0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P10DIR = 0xFF;
	
    //Timer A0 Setup
    TA0CTL |= TASSEL_1 + MC_1;
    TA0CCR0 = 32768;
    TA0CCTL0 = CCIE; // enable timer A0 interrupt


//    TA1CTL |= TASSEL_2 + MC_1;
    TA1CCR0 = 1048576/500;
    TA1CCTL0 = CCIE; // enable timer A0 interrupt



	__bis_SR_register(LPM0_bits + GIE);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    P10OUT = dacValues[index];
    index = (++index)%12;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer_A1(void)
{
    if(degree > 360){
        degree = 0;
    }
    P10OUT = floor(128*sin((degree*M_PI)/180));
    degree += 1;
}
