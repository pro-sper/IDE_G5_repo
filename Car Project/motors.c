/*
    Helper function for motor control
*/

#include <stdint.h>
#include <stdio.h>
#include "msp.h"
#include "uart.h"
#include "TimerA.h"
#include "led.h"
#include "Common.h"

void myDelay(int delay) {
	volatile int i;
	volatile int f;
	for (i=0; i<delay*50000; i++){
		f = i;// Do nothing
	}
}

// initializes P3.6 and P3.7 as gpio enable pins
void DC_Motor_Init(){
	P3->SEL0 &= ~BIT(6);
	P3->SEL1 &= ~BIT(6);

	P3->SEL0 &= ~BIT(7);
	P3->SEL1 &= ~BIT(7);

	P3->DIR|=BIT(6);
	P3->DIR|=BIT(7);

	P3->OUT &= ~BIT(6);
	P3->OUT &= ~BIT(7);
}

void DC_Motor_Enable(){
	P3->OUT |= BIT(6);
	P3->OUT |= BIT(7);
}

void stepper_setup(){	// Our stepper will be on Port C, pins 0:3
	//DDRC=0xFF;	// pins 37:34
	// Set SEL for I/O 
	P4->SEL0 &= ~BIT(1);
	P4->SEL1 &= ~BIT(1);
	
	P4->SEL0 &= ~BIT(2);
	P4->SEL1 &= ~BIT(2);
	
	P4->SEL0 &= ~BIT(3);
	P4->SEL1 &= ~BIT(3);
	
	P4->SEL0 &= ~BIT(4);
	P4->SEL1 &= ~BIT(4);

	// Set as output
	P4->DIR|=BIT(1);
	P4->DIR|=BIT(2);
	P4->DIR|=BIT(3);
	P4->DIR|=BIT(4);

	// Set all as off
	P4->OUT&= ~BIT(1);
	P4->OUT&= ~BIT(2);
	P4->OUT&= ~BIT(3);
	P4->OUT&= ~BIT(4);
}
