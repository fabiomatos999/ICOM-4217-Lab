#include <msp430.h>
#include <string.h>

void init()
{
    WDTCTL = WDTPW | WDTHOLD;       // stop watchdog timer
    P3DIR = 0xF0; // P3.4 = E, P3.5 = RS, P3.6 = R/W, P3.7 = D7
    P4DIR = 0xEF; //P4.0 = D0, P4.1 = D1, P4.2 = D2, P4.3 = D3, P4.5 = D4, P4.6 = D5, P4.7 = D6
    volatile unsigned int j;
    __delay_cycles(450000);
    P3OUT ^= 0x10;
    P4OUT = 0x60; //Function Set
    P3OUT ^= 0x10;
    __delay_cycles(450000);
    P3OUT ^= 0x10; //Function Set
    P3OUT ^= 0x10;
    __delay_cycles(4500);

    P3OUT ^= 0x10; //Function Set
    P3OUT ^= 0x10;
    __delay_cycles(4500);

    P3OUT ^= 0x10;
    P4OUT = 0x68; //Function Set with 2 lines
    P3OUT ^= 0x10;
    __delay_cycles(4500);

    __delay_cycles(4500);
    P3OUT ^= 0x10;
    P4OUT = 0x08; //Display Off
    P3OUT ^= 0x10;
    __delay_cycles(4500);
    P3OUT ^= 0x10;
    P4OUT = 0x01; // Clear Display
    P3OUT ^= 0x10;
    __delay_cycles(4500);
    P3OUT ^= 0x10;
    P4OUT = 0x06; //Entry Mode Set
    P3OUT ^= 0x10;
    __delay_cycles(4500);
    P3OUT ^= 0x10;
    P4OUT = 0x0F; //Display ON/OFF
    P3OUT ^= 0x10;
}
void return_home_top_display()
{
    P3OUT = 0x80;
    P4OUT = 0x00; //Sets address counter to 0x00 and toggles enable
    P3OUT ^= 0x10;
    P3OUT ^= 0x10;
}
void return_home_bottom_display()
{
    P3OUT = 0x80;
    P4OUT = 0x80; //Sets address counter to 0x40H and toggles enable
    P3OUT ^= 0x10;
    P3OUT ^= 0x10;
}
void input_char(char character)
{

    P3OUT = 0x20;
    P4OUT = 0x00;
    P3OUT |= ((((int) character) >> 7) & 1) << 7;
    P4OUT |= ((((int) character) >> 6) & 1) << 7;
    P4OUT |= ((((int) character) >> 5) & 1) << 6;
    P4OUT |= ((((int) character) >> 4) & 1) << 5;
    P4OUT |= ((((int) character) >> 3) & 1) << 3; // Given a character, will enable RS, and send nth bit of a char to Dn, toggles enable and resets D0-D7
    P4OUT |= ((((int) character) >> 2) & 1) << 2;
    P4OUT |= ((((int) character) >> 1) & 1) << 1;
    P4OUT |= ((((int) character) >> 0) & 1) << 0;
    P3OUT ^= 0x10;
    P3OUT ^= 0x10;
    P4OUT = 0x00;
}
void input_command(int cmd)
{

    P3OUT = 0x00;
    P4OUT = 0x00;
    P3OUT |= ((((int) cmd) >> 7) & 1) << 7;
    P4OUT |= ((((int) cmd) >> 6) & 1) << 7;
    P4OUT |= ((((int) cmd) >> 5) & 1) << 6;
    P4OUT |= ((((int) cmd) >> 4) & 1) << 5;
    P4OUT |= ((((int) cmd) >> 3) & 1) << 3; // Given a character, will enable RS, and send nth bit of a char to Dn, toggles enable and resets D0-D7
    P4OUT |= ((((int) cmd) >> 2) & 1) << 2;
    P4OUT |= ((((int) cmd) >> 1) & 1) << 1;
    P4OUT |= ((((int) cmd) >> 0) & 1) << 0;
    P3OUT ^= 0x10;
    P3OUT ^= 0x10;
    P4OUT = 0x00;
}
void print_string(char *string)
{
    unsigned int i;
    for (i = 0; i < strlen(string); i++)
    {
        input_char(string[i]); // Given a char array (string) will print out the string character by character
    }
}
void clear_display()
{
    int j;
    char *space = " ";
    for (j = 0; j < 128; j++)
    {
        input_char(space[0]); // Will input a space character on all 80 spaces in the address counter and will reset cursor to the home of the top display
    }
    return_home_top_display();
}
void button_increment(char **messages, volatile unsigned int *i)
{
    clear_display();
    return_home_top_display();
    print_string(messages[*i]);
    *i = ((*i) + 1) % 16;
    return_home_bottom_display();
    print_string(messages[*i]);
}
void button_decrement(char **messages, volatile unsigned int *i)
{
    clear_display();
    return_home_bottom_display();
    print_string(messages[*i]);
    *i = ((*i) - 1) % 16;
    return_home_top_display();
    print_string(messages[*i]);
}
