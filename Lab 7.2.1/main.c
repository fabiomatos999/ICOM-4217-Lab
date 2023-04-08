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

    }
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(){
    unsigned volatile int curr = index;
   volatile char currchar = hello[index];
    switch(__even_in_range(UCA0IV,4)){
    case 0: break;
    case 2: break;
    case 4:
        if (index < BUFF_SIZE){
            UCA0TXBUF = hello[index];
            index+=1;
        }
        else {
            UCA0TXBUF = (char) 13;
        }
        break;
    default: break;
    }
//    __bic_SR_register_on_exit(LPM0_bits);
}
