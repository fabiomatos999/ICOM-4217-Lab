#include <msp430.h> 
#include <display.h>
/**
 * main.c
 */

unsigned int voltage = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    P6SEL |= BIT7;
    ADC12CTL0 |= ADC12SHT0_15 + ADC12ON;
    ADC12CTL1 = ADC12SHP;
    ADC12MCTL0 |= ADC12INCH_7 + ADC12SREF_7;
    ADC12IE |= BIT0;
    ADC12CTL0 |= ADC12ENC;
    TA0CTL |= TASSEL_1 + MC_1; // Select SMCLK as the clock source and put the timer into Up mode
    TA0CCR0 = 32768;  // Set the value in TA0CCR0 to achieve a frequency of 50Hz
    TA0CCTL0 |= CCIE;
    __delay_cycles(37);
    __bis_SR_register(LPM4_bits + GIE);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){
    ADC12CTL0 |= ADC12SC;
}


#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR(void)
{
    voltage = (3300.0/4095.0)*ADC12MEM0;      // Move results, IFG is cleared
    volatile unsigned int v = voltage;
    __bic_SR_register_on_exit(LPM4_bits);   // Exit active CPU
}
