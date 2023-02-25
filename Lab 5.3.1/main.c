#include <msp430.h> 
#include "keypad.h"
#include <stdio.h>

#define PERIOD 1000
char BUFFER[5] = "0%\0";
void display_brightness_if_pressed();
void adjust_brightness();
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	init();
	keypad_init();
    //Timer A0 Setup
    TA0CTL |= TASSEL_2 + MC_1;
    TA0CCR0 = PERIOD; // 1KHz

    //Timer A0 CC1 Setup
    P1DIR |= BIT2;
    P1SEL |= BIT2;
    TA0CCTL1 |= OUTMOD_7;
    TA0CCR1 = 0;
    clear_display();
    return_home_top_display();
    print_string(BUFFER);

	while(1){
	    display_brightness_if_pressed();
	    P10OUT = P10OUT << 1;
        display_brightness_if_pressed();
	    P10OUT = P10OUT << 1;
        display_brightness_if_pressed();
	    P10OUT = P10OUT << 1;
        display_brightness_if_pressed();
	    P10OUT = P10OUT >> 3;
	}
}

void adjust_brightness(){
    switch (current_key) {
        case '0':
            TA0CCR1 = 0;
            break;
        case '1':
            TA0CCR1 = PERIOD*0.1;
            break;
        case '2':
            TA0CCR1 = PERIOD*0.2;
            break;
        case '3':
            TA0CCR1 = PERIOD*0.3;
            break;
        case '4':
            TA0CCR1 = PERIOD*0.4;
            break;
        case '5':
            TA0CCR1 = PERIOD*0.5;
            break;
        case '6':
            TA0CCR1 = PERIOD*0.6;
            break;
        case '7':
            TA0CCR1 = PERIOD*0.7;
            break;
        case '8':
            TA0CCR1 = PERIOD*0.8;
            break;
        case '9':
            TA0CCR1 = PERIOD*0.9;
            break;
        case '#':
            TA0CCR1 = PERIOD;
            break;
    }

}
void display_brightness_if_pressed(){
    if (pressed >= 1){
        clear_display();
        return_home_top_display();
        adjust_brightness();
        sprintf(BUFFER,"%d%c",(unsigned int)((((float)TA0CCR1)/((float)(PERIOD)))*100.0), '%');
        print_string(BUFFER);
        pressed = 0;
    }
}
