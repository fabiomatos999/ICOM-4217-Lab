#include <msp430.h> 
#include <display.h>

/**
 * main.c
 */
int main(void)
{
    init();
    clear_display();
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P3SEL |=0x06;               //Selects Ports 3.2 and P3.1 for SDA and SCL
	UCB0CTL1 ^= 0x01;           //Sets UCSWRST (Software reset enable) to 1 in order to modify the USCI module (Reset Enabled)
	UCB0CTL1 ^= 0x80;           //Sets UCSSELx to 10b to use SMCLK and UCTR to Receiver mode (since transmitter mode is not used)
	UCB0CTL0 = 0x0F;            //Sets UCMST to Master mode, UCMODEx to I2C mode, and UCSYNC to Synchronous mode
	UCB0BR0=12;                 //Sets Baud rate, using 1.048Mhz/12=100kHz
	UCB0BR1=0;
	UCB0I2CSA=0x68;             //Slave address from DS1307 (found from datasheet under "DATA WRITE - SLAVE RECIEVER MODE" on Figure 6)
	UCB0CTL1 &= ~0x01;          //Disables UCSWRST (Reset Disabled)
	IE2 |=UCB0RXIE;             //Enables interrupts for I2C

	RXData=0x00;                //To hold received data

	while(1){
	    UCB0CTL1 |=UCTXSTT;     //START condition sent
	    __bis_SR_register(CPUOFF + GIE);
	}
	
}

#pragma vector= USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
    RXData= UCB0RXBUF;          //Pass data from RX buffer to RXData

}
