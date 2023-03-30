#include <msp430.h> 

void moveFullStep();
void moveHalfStep();

typedef struct input {
    unsigned int a:1;
    unsigned int abar:1;
    unsigned int b:1;
    unsigned int bbar:1;
} input;

input fullstep[4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
input halfstep[8] = {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,1,1},{0,0,0,1},{1,0,0,1}};

unsigned int currstep = 0;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    TA0CTL |= TASSEL_1 + MC_1 + TAIE;
    TA0CCR0 = 32768/100;
	moveFullStep();
    _bis_SR_register(LPM0_bits);
}

void moveFullStep(){
    currstep = (++currstep)%4;
    P10OUT = (fullstep[currstep].a<<0) + (fullstep[currstep].b<<1) + (fullstep[currstep].abar<<2) + (fullstep[currstep].bbar<<3);
}

void moveHalfStep(){
    currstep = (++currstep)%8;
    P10OUT = (halfstep[currstep].a<<0) + (halfstep[currstep].b<<1) + (halfstep[currstep].abar<<2) + (halfstep[currstep].bbar<<3);
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){
    moveFullStep();
}
