#include <msp430.h> 

unsigned int index = 0;
#define BUFF_SIZE 13
char hello[BUFF_SIZE] = "Hello World!\n";

int main(){
    P3SEL = BIT4 + BIT5;
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;
    UCA0BR0 = 109;
    UCA0BR1 = 0x00;
    UCA0MCTL = UCBRS_2+UCBRF_0;
    UCA0CTL1 &= ~UCSWRST;
    UCA0IE |= UCTXIE;
    __enable_interrupt();
    while(1){
        __delay_cycles(4294967295);
    }
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(){
    switch(__even_in_range(UCA0IV,4)){
    case 0: break;
    case 2: break;
    case 4:
        if (index < BUFF_SIZE){
            UCA0TXBUF = hello[index];
            index+=1;
        }
        else if (index == BUFF_SIZE) {
            UCA0TXBUF = (char) 13;
            index+=1;
        }
        else {
            UCA0TXBUF = 0;
            UCA0RXBUF = 0;
            UCA0IE = 0;

        }
    default: break;
    }
}
