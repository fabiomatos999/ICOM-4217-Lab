#include <msp430.h>


void init(){
    volatile unsigned int j;
    for(j=0; j < 50000; j++);
    P3OUT ^= 0x10;
    P4OUT = 0x60;
    P3OUT ^= 0x10;
    for(j=0; j < 4500; j++);
    P3OUT ^= 0x10;
    P3OUT ^= 0x10;
    for(j=0; j < 450; j++);

    P3OUT ^= 0x10;
    P4OUT = 0x68;
    P3OUT ^= 0x10;

    P3OUT ^= 0x10;
    P4OUT = 0x08;
    P3OUT ^= 0x10;

    P3OUT ^= 0x10;
    P4OUT = 0x01;
    P3OUT ^= 0x10;

    P3OUT ^= 0x10;
    P4OUT = 0x06;
    P3OUT ^= 0x10;

    P3OUT ^= 0x10;
    P4OUT = 0x0D;
    P3OUT ^= 0x10;

}
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P3DIR = 0xF0;
	P4DIR= 0xEF;
	P3OUT = 0x00;
	init();


	volatile unsigned int i;		// volatile to prevent optimization

	while (1) {

	}
}
