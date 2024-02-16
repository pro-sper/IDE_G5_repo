#include "msp.h"
#include "Common.h"

void LED1_Init(void){
	// configure PortPin for LED1 as port I/O 
		P1->SEL0 &= ~BIT0; // Select Port1 Bit0 for use as port I/O
		P1->SEL1 &= ~BIT0; // Select Port1 Bit0 for use as port I/O
	
		P1->DIR|=BIT0; // set BIT 0 as output

	// make built-in LED1 LED high drive strength
		P1->DS|=BIT0; // set high drive strength
	//Initialize bit to '1'	
		P1->OUT|=BIT0; // set P1.0 to '1'
		P1->OUT&=~BIT0; // set P1.0 to '0'

	// make built-in LED1 out	 
		P1->OUT|=BIT0;

	// turn off LED
		P1->OUT&=~BIT0;
}

void LED2_Init(void){
	// configure PortPin for LED2 as port I/O 
		P2->SEL0 &= ~BIT0;	// Red LED
		P2->SEL1 &= ~BIT0;	// Red LED
		P2->SEL0 &= ~BIT1;  // Green LED
		P2->SEL1 &= ~BIT1;	// Green LED
		P2->SEL0 &= ~BIT2;	// Blue LED
		P2->SEL1 &= ~BIT2;	// Blue LED
		
		P2->DIR|=BIT0;
		P2->DIR|=BIT1;
		P2->DIR|=BIT2;

	// make built-in LED2 LEDs high drive strength
		P2->DS|=BIT0;
		P2->DS|=BIT1;
		P2->DS|=BIT2;
	
	// make built-in LED2 out	 
		P2->OUT|=BIT0;
		P2->OUT|=BIT1;
		P2->OUT|=BIT2;

	// turn off LED
		P2->OUT&=~BIT0;
		P2->OUT&=~BIT1;
		P2->OUT&=~BIT2;
}

void LED1_on(void){
	P1->OUT|=BIT0;
}

void LED1_off(void){
	P1->OUT&=~BIT0;
}

void LED2_R_on(void){
	P2->OUT|=BIT0;;
	P2->OUT&=~BIT1;;
	P2->OUT&=~BIT2;
}

void LED2_G_on(void){
	P2->OUT&=~BIT0;;
	P2->OUT|=BIT1;;
	P2->OUT&=~BIT2;
}

void LED2_B_on(void){
	P2->OUT&=~BIT0;
	P2->OUT&=~BIT1;
	P2->OUT|=BIT2;
}

void LED2_off(void){
	P2->OUT&=~BIT0;
	P2->OUT&=~BIT1;
	P2->OUT&=~BIT2;
}

