#include <msp430.h> 
#include <display.h>
#include <stdio.h>

/**
 * main.c
 */

typedef enum {
    Celsius,
    Fahrenheit
} Scale;

#define TEMPSIZE 10

float average_celsius();
float average_fahrenheit();

char BUFFER[17];

void resetBuffer(){
    unsigned int i = 0;
    for(i=0; i<17;i++){
        BUFFER[i] = '\0';
    }
}

Scale scale = Celsius;

float tempts[TEMPSIZE];
unsigned int index = 0;

unsigned int voltage = 0;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    init();
    clear_display();
    return_home_top_display();

    P6SEL |= BIT7;
    ADC12CTL0 |= ADC12SHT0_15 + ADC12ON;
    ADC12CTL1 = ADC12SHP;
    ADC12MCTL0 |= ADC12INCH_7 + ADC12SREF_7;
    ADC12IE |= BIT0;
    ADC12CTL0 |= ADC12ENC;

    P1DIR |= BIT2;
    P1SEL |= BIT2;

    TA0CTL |= TASSEL_1 + MC_1;
    TA0CCR0 = 32768/10;
    TA0CCTL0 |= CCIE;

    P1REN |= BIT4;
    P1IFG &= ~BIT4;
    P1IES |= BIT4;
    P1IE |= BIT4;

    __delay_cycles(37);
    __bis_SR_register(LPM4_bits + GIE);

}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){
    __delay_cycles(37);
    ADC12CTL0 |= ADC12SC;
}

#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR(void)
{
    voltage = ADC12MEM0;
    tempts[index] = (voltage-775)/10.0;
    index = (++index)%TEMPSIZE;
    resetBuffer();
    clear_display();
    return_home_top_display();
    if (scale == Celsius){
        sprintf(BUFFER,"Temp: %d C",(int)average_celsius());

    }
    else if (scale == Fahrenheit){
        sprintf(BUFFER,"Temp: %d F", (int)average_fahrenheit());

    }
    __bic_SR_register_on_exit(LPM4_bits);   // Exit active CPU
}

float average_celsius(){
    float total= 0;
    unsigned int i = 0;
    for(;i<TEMPSIZE;i++){
        total +=tempts[i];
    }
    return total/TEMPSIZE;
}
float average_fahrenheit(){
    return (average_celsius()*1.8)+32;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1()
{
    if (scale == Celsius){
        scale = Fahrenheit;
    }
    else if (scale == Fahrenheit){
        scale = Celsius;
    }
    P1IFG &= ~BIT4;
}
