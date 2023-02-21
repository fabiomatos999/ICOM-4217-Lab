#include <msp430.h> 
#include "display.h"

/**
 * main.c
 */

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


    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    init();
    return_home_top_display();
    P1DIR = 0x00;
//    P1REN = 0x30;
    P1IFG &= ~0x30;
    P1IE = 0x30;
    __enable_interrupt();

    _BIS_SR(LPM0_bits);
}

void move_left_or_right(unsigned int curr_index)
{
    if(curr_index == 3 && index == 0){
        increment_messages_index();
        button_increment(messages,&messages_index);
    }
    else if (curr_index == 0 && index == 3){
        decrement_messages_index();
        button_decrement(messages,&messages_index);
    }
    else if (curr_index > index)
    {
        increment_messages_index();
        button_increment(messages,&messages_index);
    }
    else if (curr_index < index)
    {
        decrement_messages_index();
        button_decrement(messages,&messages_index);
    }
    index = curr_index;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1()
{
    unsigned volatile int outer = (P1IFG >> 5)&1;
    unsigned volatile int inner = (P1IFG >> 4)&1;
    unsigned int index = find_index(outer, inner);
    move_left_or_right(index);
    if (outer)
    {
        P1IFG &= ~0x20;
    }
    if (inner)
    {
        P1IFG &= ~0x10;
    }
}
