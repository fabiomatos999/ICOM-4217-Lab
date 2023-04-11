#include <msp430.h> 
#include <display.h>

unsigned char curr_char = ' ';

int main(){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    init();
    clear_display();
    return_home_top_display();
    P10SEL = BIT4 + BIT5;
    UCA3CTL1 |= UCSWRST;
    UCA3CTL1 |= UCSSEL_2;
    UCA3BR0 = 109;
    UCA3BR1 = 0x00;
    UCA3MCTL = UCBRS_2+UCBRF_0;
    UCA3CTL1 &= ~UCSWRST;
    UCA3IE |= UCRXIE;
    __enable_interrupt();
    __bis_SR_register(LPM0_bits);
}

#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_ISR(){
    switch(__even_in_range(UCA3IV,4)){
    case 0: break;
    case 2:
        curr_char = UCA3RXBUF;
        input_char(curr_char);
        UCA3TXBUF = curr_char;
        break;
    case 4: break;
    default: break;
    }
    __bic_SR_register_on_exit(LPM0_bits);
}
