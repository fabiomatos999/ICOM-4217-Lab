#include <msp430.h> 
#include <display.h>
#include <stdio.h>



#define CLK 32768/1000

float dutycycle = 0;

char BUFFER[17];

void resetBuffer(){
    unsigned int i = 0;
    for(i=0; i<17;i++){
        BUFFER[i] = '\0';
    }
}

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
    TA0CTL |= TASSEL_1 + MC_1;
    TA0CCR0 = CLK;
    TA0CCTL1 |= OUTMOD_7;
    TA0CCR1 = 0;
    P1DIR |= BIT2;
    P1SEL |= BIT2;
    ADC12CTL0 |= ADC12ENC;

    __delay_cycles(37);
    while (1){
        ADC12CTL0 |= ADC12SC;
        __bis_SR_register(LPM4_bits + GIE);
    }
}



#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR(void)
{
    dutycycle = ADC12MEM0/(4095.0);
    volatile float dc = dutycycle;
    TA0CCR1 = CLK*dutycycle;
    resetBuffer();
    clear_display();
    return_home_top_display();
    sprintf(BUFFER,"Brightness: %d%",(int)(dutycycle*100.0));
    print_string(BUFFER);
    if (dutycycle*100.0 < 0.001){
        resetBuffer();
        return_home_bottom_display();
        sprintf(BUFFER,"WARN: Voltage At MIN");
        print_string(BUFFER);

    }
    else if (dutycycle*100.0 > 99.999){
        resetBuffer();
        return_home_bottom_display();
        sprintf(BUFFER,"WARN: Voltage At MAX");
        print_string(BUFFER);
    }



    __bic_SR_register_on_exit(LPM4_bits);   // Exit active CPU
}
