//#include <msp430.h>
//#include "display.h"
//#include <stdio.h>
//#define BUFFER_SIZE 16
//volatile unsigned int x = 0;
//char BUFFER[BUFFER_SIZE] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
//unsigned int pressed = 0;
//void reset_buffer(){
//    unsigned int i = 0;
//    for(;i<BUFFER_SIZE;i++){
//        BUFFER[i] = '\0';
//    }
//}
//int main()
//{
//    init();
//    __enable_interrupt();
//    P1DIR = 0x00;
//    P1REN = 0x04;
//    P1IFG &= ~0x04;
//    P1IES = 0x04;
//    P1IE = 0x04;
//    while (1)
//    {
//        if(pressed >= 1){
//            x += 1;
//            reset_buffer();
//            clear_display();
//            return_home_top_display();
//            sprintf(BUFFER, "%d", x);
//            print_string(BUFFER);
//            pressed = 0;
//            __delay_cycles(240000);
//        }
//    }
//}
//#pragma vector=PORT1_VECTOR
//__interrupt void PORT_1()
//{
//    pressed = 1;
//    P1IFG &= ~0x04;
//}
