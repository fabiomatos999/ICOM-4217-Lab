#include <msp430.h> 
#include <display.h>

unsigned char curr_char = ' ';
unsigned char message[17];
unsigned int index = 0;

void resetBuffer();


int main(){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    resetBuffer();
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
        message[index] = curr_char;
        index +=1;
        if (index > 16)
        {
            index = 0;
            UCA3IE |= UCTXIE;
        }
        break;
    case 4:
        if (index < 16){
            UCA3TXBUF = (unsigned char)(message[index]-32);
            index +=1;
        }
        else if (index == 17){
            UCA3TXBUF = '\n';
            index +=1;
        }
        else {
            UCA3TXBUF = '\r';
            resetBuffer();
            index = 0;
            UCA3IE &= ~UCTXIE;
        }
        break;
    default: break;
    }
    __bic_SR_register_on_exit(LPM0_bits);
}

void resetBuffer(){
    int i = 0;
    for(i=0;i<16;i++){
        message[i] = '\0';
    }
}
