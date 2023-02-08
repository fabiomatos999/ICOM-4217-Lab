#include <msp430.h>


void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer

    P4REN |= 0x02;                  //Place P4.1 as pull-up (HIGH), place P4.3 as pull-dwon (LOW)
    P4OUT = 0x02;                   //Pull-up LED for P4.0 meaning LED set HIGH, P4.2 set Pull-down meaning LED set LOW
    P4DIR = 0x05;                  // configure P4.0 and P4.2 as outputs (HIGH), P4.1 and P4.3 as inputs (LOW)

    volatile unsigned int i;        // volatile to prevent optimization

    while(1)
    {

        if(!(P4IN & 0x02)){
            P4OUT ^= 0x01;
        }

        if(P4IN & 0x08){
            P4OUT^=0x04;
        }
    }
}
