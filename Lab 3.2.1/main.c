#include <msp430.h>
#include "display.h"
#include <stdio.h>
#define BUFFER_SIZE 16
volatile unsigned int x = 0;
char BUFFER[BUFFER_SIZE] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
void reset_buffer(){
    unsigned int i = 0;
    for(;i<BUFFER_SIZE;i++){
        BUFFER[i] = '\0';
    }
}
int main()
{
    init();
    clear_display();
    return_home_top_display();
    __enable_interrupt();
    P1DIR = 0x00;
    P1REN = 0x04;
    P1IFG &= ~0x04;
    P1IES = 0x04;
    P1IE = 0x04;
    while (1)
    {

    }
}
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1()
{
    x += 1;
    reset_buffer();
    clear_display();
    return_home_top_display();
    sprintf(BUFFER, "%d", x);
    print_string(BUFFER);
    P1IFG &= ~0x04;
}
