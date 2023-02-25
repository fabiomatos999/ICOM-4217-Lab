#include <msp430.h>
#include "display.h"
char keypad_keys[12] = { '1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0',
                         '#' };
char current_key = 'a';
unsigned int pressed = 0;
unsigned int cursor_position = 0;
void input_if_pressed()
{
    if (pressed >= 1)
    {
        if (current_key != '*' && current_key != '#')
        {
            input_char(current_key);
            cursor_position += 1;
        }
        pressed = 0;
        __delay_cycles(250000);
    }
    __delay_cycles(5000);
}

void keypad_init(){
    P1DIR &= ~0x70; // P1.4 = Column 1, P1.5 = Column 2, P1.6 = Column 3
    P1REN |= 0x70;
    P1IFG &= ~0x70;
    P1IES |= 0x70;
    P1IE |= 0x70;
    P10DIR |= 0x0F; // P10.0 = Row 1, P10.1 = Row 2, P10.2 = Row 3, P10.3 = Row 4
    P10OUT = 0x01;
    __enable_interrupt();
}

void scan(){
    input_if_pressed();
    P10OUT = P10OUT << 1;
    input_if_pressed();
    P10OUT = P10OUT << 1;
    input_if_pressed();
    P10OUT = P10OUT << 1;
    input_if_pressed();
    P10OUT = P10OUT >> 3;
}

void update_position(){
    if (cursor_position <= 0x0F){
        cursor_position += 0x40;
    }
    else if (cursor_position > 0x0F && cursor_position < 0x40){
        return_home_bottom_display();
        cursor_position = 0x40;
    }
    else if (cursor_position >= 0x40 && cursor_position <= 0x4F){
        cursor_position -= 0x40;
    }
    else if (cursor_position > 0x4F){
        return_home_top_display();
        cursor_position = 0x00;
    }
}

char decode_pad(unsigned volatile int column, int port)
{
    char decoded_char = NULL;
    if (column == 1)
    {
        switch (port)
        {
        case 0x02:
            decoded_char = keypad_keys[0];
            break;
        case 0x04:
            decoded_char = keypad_keys[3];
            break;
        case 0x08:
            decoded_char = keypad_keys[6];
            break;
        case 0x01:
            decoded_char = keypad_keys[9];
            break;
        }
    }
    else if (column == 2)
    {
        switch (port)
        {
        case 0x02:
            decoded_char = keypad_keys[1];
            break;
        case 0x04:
            decoded_char = keypad_keys[4];
            break;
        case 0x08:
            decoded_char = keypad_keys[7];
            break;
        case 0x01:
            decoded_char = keypad_keys[10];
            break;
        }
    }
    else if (column == 3)
    {
        switch (port)
        {
        case 0x02:
            decoded_char = keypad_keys[2];
            break;
        case 0x04:
            decoded_char = keypad_keys[5];
            break;
        case 0x08:
            decoded_char = keypad_keys[8];
            break;
        case 0x01:
            decoded_char = keypad_keys[11];
            break;
        }
    }
    return decoded_char;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1()
{
    unsigned volatile int column1 = (P1IFG >> 4) & 1;
    unsigned volatile int column2 = (P1IFG >> 5) & 1;
    unsigned volatile int column3 = (P1IFG >> 6) & 1;
    unsigned volatile int column = 0;
    if (column1)
    {
        column = 1;
    }
    else if (column2)
    {
        column = 2;
    }
    else if (column3)
    {
        column = 3;
    }
    else
    {
        return;
    }
    current_key = decode_pad(column, P10OUT);
    if (current_key == NULL)
    {
        return;
    }
    pressed = 1;
    P1IFG &= ~0x70;
}
