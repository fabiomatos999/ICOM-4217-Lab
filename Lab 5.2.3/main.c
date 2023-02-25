#include <msp430.h> 

#define PERIOD 1000

typedef struct RGB{
    unsigned int r:8;
    unsigned int g:8;
    unsigned int b:8;
}RGB;

void set_RGB_LED(RGB rgb);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //Timer A0 Setup
    TA0CTL |= TASSEL_2 + MC_1;
    TA0CCR0 = PERIOD; // 1KHz

    //Timer A0 CC1 Setup (RED)
    P1DIR |= BIT2;
    P1SEL |= BIT2;
    TA0CCTL1 |= OUTMOD_7;
    TA0CCR1 = 0;

    //Timer A0 CC1 Setup (GREEN)
    P1DIR |= BIT3;
    P1SEL |= BIT3;
    TA0CCTL2 |= OUTMOD_7;
    TA0CCR2 = 0;

    //Timer A0 CC1 Setup (BLUE)
    P1DIR |= BIT4;
    P1SEL |= BIT4;
    TA0CCTL3 |= OUTMOD_7;
    TA0CCR3 = 0;

    RGB rgb = {255,255,255};
    set_RGB_LED(rgb);

    _bis_SR_register(LPM0_bits);

}

void set_RGB_LED(RGB rgb){
    TA0CCR1 = (unsigned int)((((float)(rgb.r))/255.0)*PERIOD);
    TA0CCR2 = (unsigned int)((((float)(rgb.g))/255.0)*PERIOD);
    TA0CCR3 = (unsigned int)((((float)(rgb.b))/255.0)*PERIOD);
}
