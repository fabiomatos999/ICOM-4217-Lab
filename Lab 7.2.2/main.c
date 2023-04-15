#include <msp430.h> 
#include <display.h>

unsigned char curr_char = ' ';
unsigned char message[16];
unsigned int index = 0;

unsigned int position = 0;
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
        position+=1;
        if (position > 0x0F && position < 0x40){
            return_home_bottom_display();
            position = 0x40;
        }
        else if (position > 0x4F){
            return_home_top_display();
            position = 0x00;
        }
        if (index > 16)
        {
            index = 0;
            UCA3IE |= UCTXIE;
        }
        break;
    case 4:
        if (index < 15){
            UCA3TXBUF = (unsigned char)(message[index]-32);
            index +=1;
        }
        else {
            UCA3TXBUF = '\n';
            __delay_cycles(100000);
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
