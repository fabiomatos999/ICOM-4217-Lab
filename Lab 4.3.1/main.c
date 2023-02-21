#include <msp430.h> 
#include "display.h"
#include <stdio.h>


/**
 * main.c
 */
char BUFFER[13] = "Speed=000RPM\0";
typedef struct photodiodeState
{
    unsigned int outer :1;
    unsigned int inner :1;
} photodiodeState;
photodiodeState state[4] = { { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 } };
unsigned volatile int index = 0;
unsigned int messages_index = 0;
char *messages[16] = { "UwU1", "UwU2", "UwU3", "UwU4", "UwU5", "UwU6",
                       "UwU7", "UwU8", "UwU9", "UwU10", "UwU11", "UwU12",
                       "UwU13", "UwU14", "UwU15", "UwU16" };
unsigned int ammount_changes = 0;
void increment_messages_index(){
    messages_index = (++messages_index)%16;
}
void decrement_messages_index(){
    messages_index = (--messages_index)%16;
}
unsigned int find_index(unsigned int outer, unsigned int inner)
{
    unsigned int i = 0;
    unsigned int ret = 0;
    for (; i < 4; i++)
    {
        if (state[i].outer == outer && state[i].inner == inner)
        {
            ret = i;
            break;
        }
    }
    return ret;
}
void main(void)
{


    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    init();
    return_home_top_display();
    clear_display();
    P1DIR = 0x00;
    P1IFG &= ~0x30;
    P1IE = 0x30;
    TA0CTL |= 0x110; // Setting Timer A0 using ACLK with 1 prescaler
    TA0CCTL0 |= 0x10;
    TA0CCR0 = 0x8000;
    __enable_interrupt();

    _BIS_SR(LPM0_bits);
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1()
{
    unsigned volatile int outer = (P1IFG >> 5)&1;
    unsigned volatile int inner = (P1IFG >> 4)&1;
    ammount_changes += 1;
    if (outer)
    {
        P1IFG &= ~0x20;
    }
    if (inner)
    {
        P1IFG &= ~0x10;
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){
    return_home_top_display();
    clear_display();
    sprintf(BUFFER,"Speed=%dRPM",ammount_changes);
    print_string(BUFFER);
    ammount_changes = 0;
}
