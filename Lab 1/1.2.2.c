#include <msp430.h>
#include <string.h>

void clear_display(){
    check_busy();
    int j;
    char* space = " ";
    for(j=0; j< 80;j++){
        input_char(space[0]); // Will input a space character on all 80 spaces in the address counter and will reset cursor to the home of the top display
    }
    return_home_top_display();
}

void check_busy(){
//    P3DIR &= 0x7F;
//    P3OUT ^= 0x10;
//    P3OUT |= 0x04;
//    while(P3IN & 0x80){} //Changes D7 so input and waits while busy bit is 1 
//    P3OUT ^= 0x10;
//    P3DIR |= 0x80;

}

void init(){
    volatile unsigned int j;
    for(j=0; j < 50000; j++);
    check_busy();
    P3OUT ^= 0x10;
    P4OUT = 0x60; //Function Set
    P3OUT ^= 0x10;
    for(j=0; j < 4500; j++);
    check_busy();
    P3OUT ^= 0x10;//Function Set
    P3OUT ^= 0x10;
    for(j=0; j < 450; j++);
    check_busy();
    P3OUT ^= 0x10;//Function Set
    P3OUT ^= 0x10;

    check_busy();
    P3OUT ^= 0x10;
    P4OUT = 0x68; //Function Set with 2 lines
    P3OUT ^= 0x10;

    check_busy();
    P3OUT ^= 0x10;
    P4OUT = 0x08; //Display Off
    P3OUT ^= 0x10;

    check_busy();
    P3OUT ^= 0x10;
    P4OUT = 0x01; // Clear Display
    P3OUT ^= 0x10;

    check_busy();
    P3OUT ^= 0x10;
    P4OUT = 0x06; //Entry Mode Set
    P3OUT ^= 0x10;

    check_busy();
    P3OUT ^= 0x10;
    P4OUT = 0x0F; //Display ON/OFF
    P3OUT ^= 0x10;
}
void return_home_top_display(){
    P3OUT = 0x80;
    P4OUT = 0x00; //Sets addres counter to 0x00 and toggles enable
    P3OUT ^= 0x10;
    P3OUT ^= 0x10;
}
void return_home_bottom_display(){
    P3OUT = 0x80;
    P4OUT = 0x80; //Sets address counter to 0x40H and toggles enable
    P3OUT ^= 0x10;
    P3OUT ^= 0x10;
}
void input_char(char character){
    check_busy();
    P3OUT = 0x20;
    P4OUT = 0x00;
    P3OUT |= ((((int)character)>>7)&1)<<7;
    P4OUT |= ((((int)character)>>6)&1)<<7;
    P4OUT |= ((((int)character)>>5)&1)<<6;
    P4OUT |= ((((int)character)>>4)&1)<<5;
    P4OUT |= ((((int)character)>>3)&1)<<3; // Given a character, will enable RS, and send nth bit of a char to Dn, toggles enable and resets D0-D7
    P4OUT |= ((((int)character)>>2)&1)<<2;
    P4OUT |= ((((int)character)>>1)&1)<<1;
    P4OUT |= ((((int)character)>>0)&1)<<0;
    P3OUT ^= 0x10;
    P3OUT ^= 0x10;
    P4OUT = 0x00;
    P3OUT ^= 0x00;
}
void print_string(char* string){
    unsigned int i;
    for(i=0;i<strlen(string);i++){
        input_char(string[i]); // Given a char array (string) will print out the string character by character
    }
}
void main(void)
{

    WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P3DIR = 0xF0; // P3.4 = E, P3.5 = RS, P3.6 = R/W, P3.7 = D7
	P4DIR= 0xEF; //P4.0 = D0, P4.1 = D1, P4.2 = D2, P4.3 = D3, P4.5 = D4, P4.6 = D5, P4.7 = D6
	P3OUT = 0x00;
	init();
	return_home_top_display();

	char*messages[16] = {"UwU1","UwU2","UwU3","UwU4",
                             "UwU5","UwU6","UwU7","UwU8",
                             "UwU9","UwU10","UwU11","UwU12",
                             "UwU13","UwU14","UwU15","UwU16"};
	volatile unsigned int i=0;		// volatile to prevent optimization

	while (1) {

	        clear_display();
	        return_home_top_display();
	        print_string(messages[i]);
	        i= (i+1)%16;
	        return_home_bottom_display();
	        print_string(messages[i]);
	        unsigned int j;
	        for(j=0;j<50000;j++){}
	    }
}
