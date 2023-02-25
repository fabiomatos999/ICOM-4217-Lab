#include <msp430.h> 

#define PERIOD 1000

typedef struct RGB
{
    unsigned int r :8;
    unsigned int g :8;
    unsigned int b :8;
} RGB;

void set_RGB_LED(RGB rgb);

RGB color_values[8] = { {0,0,255},
                        {0,255,0},
                        {255,0,0},
                        {255,30,217},
                        {30,222,252},
                        {240,200,40},
                        {255,123,33},
                        {255,255,255}};

unsigned index = 0;
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

    // Pushbutton Setup
    P1IES |= BIT5;
    P1IFG &= ~BIT5;
    P1REN |= BIT5;
    P1IE |= BIT5;
    __enable_interrupt();
    set_RGB_LED(color_values[index]);

    _bis_SR_register(LPM0_bits);
}

void set_RGB_LED(RGB rgb)
{
    TA0CCR1 = (unsigned int) ((((float) (rgb.r)) / 255.0) * PERIOD);
    TA0CCR2 = (unsigned int) ((((float) (rgb.g)) / 255.0) * PERIOD);
    TA0CCR3 = (unsigned int) ((((float) (rgb.b)) / 255.0) * PERIOD);
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(){

    index = (++index)%8;
    set_RGB_LED(color_values[index]);
    __delay_cycles(250000);
    P1IFG &= ~BIT5;
}
