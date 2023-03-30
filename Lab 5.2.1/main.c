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

    // Configures the two buttons connected to P1
    P1REN |= BIT4 + BIT5; // Sets the pull up/down resistor mode for each pin in port 1
    P1IES |= BIT4 + BIT5; // sets the interrupt edge select for each pin
    P1IFG &= ~(BIT4 + BIT5); // Register P1IFG is cleared
    P1IE |= BIT4 + BIT5;
    __enable_interrupt();
    _bis_SR_register(LPM4_bits);

}

// Interrupt service rutine (ISR) for port 1 which checks which button caused the interrupt by reading the corresponding flag in the P1IFG.
#pragma vector=PORT1_VECTOR // Tells the compiler to place the ISR code at the location in memory reserved for port 1 interrupt vector
__interrupt void PORT_1() //__interrupt keyword is a GCC extension that indicates that the following function is an interrupt service routine.
{
    // checks what button has been pressed
    if (((P1IFG >> 4) & 1) == 1)
    {
        button_increment(messages, &i);
        __delay_cycles(250000);
        P1IFG &= ~BIT4; //toggles the flag, clearing it, ensuring that the interrupt is not triggered multiple times in one press
    }
    else if (((P1IFG >> 5) & 1) == 1)
    {
        button_decrement(messages, &i);
        __delay_cycles(250000);
        P1IFG &= ~BIT5;
    }
}

