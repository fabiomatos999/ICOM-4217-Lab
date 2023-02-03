#include <msp430.h>				


/**
 * blink.c
 */
void init(){
    for(i=0; i < 1250000; i++);
    P3OUT |= 0x40;
    P3OUT &= 0xDE;
    P4OUT |= 0x60;
    P4OUT &= 0x7F;
    P5OUT &= 0xEF;
    for(i=0; i < 125000; i++);
    P3OUT |= 0x40;
    P3OUT &= 0xDE;
    P4OUT |= 0x60;
    P4OUT &= 0x7F;
    P5OUT &= 0xEF;
    for(i=0; i < 3750; i++);
    P3OUT &= 0xDE; //
    P3OUT |= 0x80; //
    P5OUT &= 0xEF; // Function Set with 1 line and 5x8 character font
    P4OUT &= 0x80; //
    P4OUT |= 0x60; //

    P3OUT &= 0xDE; //
    P3OUT |= 0x80; //
    P5OUT &= 0xEF; //Display Off
    P4OUT &= 0x18; //
    P4OUT |= 0x08; //

    P3OUT &= 0xDE; //
    P3OUT |= 0x80; //
    P5OUT &= 0xEF; // Clear Display
    P4OUT &= 0xEE; //
    P4OUT |= 0x01; //

    P3OUT &= 0xDE; //
    P3OUT |= 0x80; //
    P5OUT &= 0xEF; // Entry mode set
    P3OUT &= 0x16; //
    P3OUT |= 0x04; //
    P3OUT |= 0x02; //

    P3OUT &= 0xDE; //
    P3OUT |= 0x80; //
    P5OUT &= 0xEF; //
    P3OUT &= 0x1F; // Display On/Off
    P3OUT |= 0x08; //
    P3OUT &= 0xF7; //


}
void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P3DIR = 0x31;  // P3.0 = R/W, P3.5 = RS, P3.6 = E
	P4DIR= 0xEF; // D0 = P4.0,D1 = P4.1, D2 = P4.2, D3 = P4.3, D4 = P4.5, D5 = P4.6, D6 = P4.7
	P5DIR = 0x10; // D7 = P5.4
	init();


	volatile unsigned int i;		// volatile to prevent optimization

	while true {
	}
}
