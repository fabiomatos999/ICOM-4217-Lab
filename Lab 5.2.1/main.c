#include <msp430.h> 
#include "display.h"

/**
 * main.c
 */
//  ####################
//  # Main without LPM #
//  ####################
//int main(void)
//{
//	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
//	init();
//	clear_display();
//	return_home_top_display();
//    char*messages[16] = {"UwU1","UwU2","UwU3","UwU4",
//                         "UwU5","UwU6","UwU7","UwU8",
//                         "UwU9","UwU10","UwU11","UwU12",
//                         "UwU13","UwU14","UwU15","UwU16"};
//
//    volatile unsigned int i=0;      // volatile to prevent optimization
//    button_increment(messages,&i);
//
//    while (1) {
//           if(!(((P1IN>>4)&1) == 1)){
//               button_increment(messages,&i);
//               __delay_cycles(50000);
//               __delay_cycles(50000);
//           }
//           if(((P1IN>>5)&1) == 1){
//               button_decrement(messages,&i);
//               __delay_cycles(50000);
//               __delay_cycles(50000);
//           }
//           __delay_cycles(50000);
//           __delay_cycles(50000);
//        }
//
//}
//  #################
//  # Main with LPM #
//  #################
char *messages[16] = { "UwU1", "UwU2", "UwU3", "UwU4", "UwU5", "UwU6", "UwU7",
                       "UwU8", "UwU9", "UwU10", "UwU11", "UwU12", "UwU13",
                       "UwU14", "UwU15", "UwU16" };

volatile unsigned int i = 0;      // volatile to prevent optimization
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    init();
    clear_display();
    return_home_top_display();
    button_increment(messages, &i);

    P1REN |= BIT4 + BIT5;
    P1IES |= BIT4 + BIT5;
    P1IFG &= ~(BIT4 + BIT5);
    P1IE |= BIT4 + BIT5;
    __enable_interrupt();
    _bis_SR_register(LPM4_bits);

}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1()
{
    if (((P1IFG >> 4) & 1) == 1)
    {
        button_increment(messages, &i);
        __delay_cycles(250000);
        P1IFG &= ~BIT4;
    }
    else if (((P1IFG >> 5) & 1) == 1)
    {
        button_decrement(messages, &i);
        __delay_cycles(250000);
        P1IFG &= ~BIT5;
    }
}

