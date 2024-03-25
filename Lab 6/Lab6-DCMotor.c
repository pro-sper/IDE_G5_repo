/*
 * Main module for testing the PWM Code for the K64F
 * 
 * Author:  
 * Created:  
 * Modified: Carson Clarke-Magrab <ctc7359@rit.edu> 
 * LJBeato
 * 2021
 */

#include <stdint.h>
#include <stdio.h>
#include "msp.h"
#include "uart.h"
#include "TimerA.h"
#include "led.h"
#include "Common.h"

#define EVER ;;

/*
	Waits for a delay (in milliseconds)
	del - The delay in milliseconds
	This assumes clock speed, actual delay may vary
*/
void myDelay(int del) {
	volatile int i;
	volatile int f;
	for (i=0; i<del*50000; i++){
		f = i;// Do nothing
	}
}

/*
	Configure the signal multiplexer for GPIO pins
	Configure the GPIO Pins for output
	P4.1 -> A, 1a
	P4.2 -> B, 2a
	P4.3 -> C, 1b
	P4.4 -> D, 2b
*/
void stepper_setup(){	// Our stepper will be on Port C, pins 0:3
	//DDRC=0xFF;	// pins 37:34
	// Set SEL for I/O 
	P1->SEL0 &= ~BIT(1);
	P1->SEL1 &= ~BIT(1);
	P1->SEL0 &= ~BIT(2);
	P1->SEL1 &= ~BIT(2);
	P1->SEL0 &= ~BIT(3);
	P1->SEL1 &= ~BIT(3);
	P1->SEL0 &= ~BIT(4);
	P1->SEL1 &= ~BIT(4);

	// Set as output
	P1->DIR|=BIT(1);
	P1->DIR|=BIT(2);
	P1->DIR|=BIT(3);
	P1->DIR|=BIT(4);

	// Set all as off
	P1->OUT&= ~BIT(1);
	P1->OUT&= ~BIT(2);
	P1->OUT&= ~BIT(3);
	P1->OUT&= ~BIT(4);
}

int main(void) {
	// Initialize UART and PWM
	// INSERT CODE HERE
	uart0_init();
	LED1_Init();
	LED2_Init();

	// Print welcome over serial
	uart0_put("\r\nRunning... \r\n");

	/* Part 1 - UNCOMMENT THIS
		Generate 20% duty cycle at 10kHz
	*/
	// goofy behavior possible due to rounding result to uint16_t
	uint16_t period = SystemCoreClock/10000;
	TIMER_A0_PWM_Init(period, 0, 1);
	TIMER_A0_PWM_Init(period, 0, 2);
	TIMER_A0_PWM_Init(period, 0, 3);
	TIMER_A0_PWM_Init(period, 0, 4);
	
	// Signoff 1 - 20% duty cycle at 10kHz
	TIMER_A0_PWM_DutyCycle(0.2, 1);
	TIMER_A0_PWM_DutyCycle(0.2, 3);

	// Signoff 2 - DC Motor Functionality
	
	/*
	for(EVER) { //loop forever
		//uint16_t dc = 0;
		//uint16_t freq = 10000; // Frequency = 10 kHz 
		//uint16_t dir = 0;
		//char c = 48;
		int i=0;
		int delay = 10;

		// 0 to 100% duty cycle in forward direction
		for (i=0; i<100; i++) {
			TIMER_A0_PWM_DutyCycle((double)i/100,1);
			myDelay(delay);
		}

		// 100% down to 0% duty cycle in the forward direction
		for (i=100; i>=0; i--) {
			TIMER_A0_PWM_DutyCycle((double)i/100,1);
			myDelay(delay);
		}

		// 0 to 100% duty cycle in reverse direction
		for (i=0; i<100; i++) {
			TIMER_A0_PWM_DutyCycle((double)i/100,3);
			myDelay(delay);
		}

		// 100% down to 0% duty cycle in the reverse direction
		for (i=100; i>=0; i--) {
			TIMER_A0_PWM_DutyCycle((double)i/100,3);
			myDelay(delay);
		}
	}
	*/

	// Signoff 3 - Stepper Motor Functionality
	// Configure the signal multiplexer for GPIO pins
	// Configure the GPIO Pins for output
	// P4.1 -> A, 1a
	// P4.2 -> B, 2a
	// P4.3 -> C, 1b
	// P4.4 -> D, 2b
	/*
	int forward = 1;
	int phase = 0;
	void stepper_setup();
	for(EVER){
		if (forward) {
			switch(phase){
				case 1:	// 1a
					P1->OUT&= ~BIT(4);
					P1->OUT|=BIT(1);
					//PORTC = 0x01;	// turn on coil A
					phase++;
					break;
				case 2:	// 2a
					P1->OUT&= ~BIT(1);
					P1->OUT|=BIT(2);
					//PORTC = 0x02;	// turn on coil B
					phase++;
					break;
				case 3:	// 1b
					P1->OUT&= ~BIT(2);
					P1->OUT|=BIT(3);
					//PORTC = 0x04;	// turn on coil C
					phase++;
					break;
				default: // 2b
					P1->OUT&= ~BIT(3);
					P1->OUT|=BIT(4);
					//PORTC = 0x08;	// turn on coil D
					phase = 0;
					break;
			}
		} else {	// reverse
			switch(phase) {
				case 0:	// 2b
					P1->OUT&= ~BIT(1);
					P1->OUT|= BIT(4);
					//PORTC = 0x08;	// turn on coil D
					phase++;
					break;
				case 1:	// 1b
					P1->OUT&= ~BIT(4);
					P1->OUT|= BIT(3);
					//PORTC = 0x04;	// turn on coil C
					phase++;
					break;
				case 2:	// 2a
					P1->OUT&= ~BIT(3);
					P1->OUT|= BIT(2);
					//PORTC = 0x02;	// turn on coil B
					phase++;
					break;
				default: // 1a
					P1->OUT&= ~BIT(2);
					P1->OUT|= BIT(1);
					//PORTC = 0x01;	// turn on coil A
					phase = 0;
					break;
			}
		}
		myDelay(20);
	}
	*/
	
	// Signoff 4 - Servo Motor Functionality
	/*
	uint16_t servo_period = SystemCoreClock/50;
	double servo_duty = 0;
	uint16_t servo_pin = 1;

	// initialize A2, turn on red led if error thrown for fun
	if (TIMER_A2_PWM_Init(servo_period, servo_duty, servo_pin) == -2){
		uart0_put("Servo init error");
		LED1_on();
	}

	servo_duty = SystemCoreClock*(1.0/20.0);
	TIMER_A2_PWM_DutyCycle(servo_duty, servo_pin);
	*/

	// Signoff 5 - Simultaneous TI Car Motors-Servo Motor

	return 0;
}
