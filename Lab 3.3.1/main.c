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
photodiodeState state[4] = {{0,0 },{ 0, 1 }, { 1, 1 }, { 1, 0 } };
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
    if (outer == 0 && inner == 0){
        return 0;
    }
    else if (outer == 0 && inner == 1){
        return 1;
    }
    else if(outer == 1 && inner == 1){
        return 2;
    }
    else {
        return 3;
    }

}
void move_left_or_right(unsigned int curr_index)
{
    if(curr_index == 0 && index == 3){
        increment_messages_index();
        button_increment(messages,&messages_index);
    }
    else if (curr_index == 3 && index == 0){
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

void main(void)
{


    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    init();
    return_home_top_display();
    P1DIR = 0x00;
    unsigned volatile int outer = 0;
    unsigned volatile int inner = 0;
    unsigned volatile int index = 0;

    while (1){
        outer = (P1IN >> 5)&1;
        inner = (P1IN >> 4)&1;
        index = find_index(outer, inner);
        move_left_or_right(index);
    }
}
