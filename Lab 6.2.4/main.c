#include <msp430.h> 

typedef enum direction {CW,CCW,NONE} direction;

void moveFullStep(direction dir);
void moveHalfStep(direction dir);
void moveDeg(unsigned int deg, direction dir);

typedef struct input {
    unsigned int a:1;
    unsigned int bbar:1;
    unsigned int abar:1;
    unsigned int b:1;
} input;



input fullstep[4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
input halfstep[8] = {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,1,1},{0,0,0,1},{1,0,0,1}};

unsigned int currstep = 0;
direction currdirr = NONE;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    TA0CTL = TASSEL_1 + MC_1;
    TA0CCTL0 = CCIE;
    TA0CCR0 = 32768/100;
    P1DIR = 0x0F;
	__enable_interrupt();
    _bis_SR_register(LPM0_bits);
}

void moveFullStep(direction dir){
    if (dir == CW) {
        currstep = (++currstep)%4;
        currdirr = CW;

    }
    else if (dir == CCW) {
        currstep = (--currstep)%4;
        currdirr = CCW;
    }
    else {
        currdirr = NONE;
        return;
    }
    volatile input step = fullstep[currstep];
    P1OUT = ~((fullstep[currstep].a<<0) + (fullstep[currstep].bbar<<1) + (fullstep[currstep].abar<<2) + (fullstep[currstep].b<<3));
}

void moveHalfStep(direction dir){
    if (dir == CW) {
        currstep = (++currstep)%8;
        currdirr = CW;

    }
    else if (dir == CCW) {
        currstep = (--currstep)%8;
        currdirr = CCW;

    }
    else {
        currdirr = NONE;
        return;
    }
    P1OUT = ~((halfstep[currstep].a<<0) + (halfstep[currstep].bbar<<1) + (halfstep[currstep].abar<<2) + (halfstep[currstep].b<<3));
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void){
    moveFullStep(currdirr);
}

void moveSteps(unsigned int deg, direction dir){
//    if(dir == CW){
//        unsigned int i = 0;
//        for(i;i<deg,15,)
//    }
}
