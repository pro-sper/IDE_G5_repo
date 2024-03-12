// TimerA.c

/*  LJBeato
    2021
    TimerA functionality to drive DC motor
    and Servo Motor
 */


#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include "TimerA.h"
#include "Uart.h"

// Make these arrays 5 deep, since we are using indexes 1-4 for the pins
static uint32_t DEFAULT_PERIOD_A0[5] = {0,0,0,0,0};
static uint32_t DEFAULT_PERIOD_A2[5] = {0,0,0,0,0};
//***************************PWM_Init*******************************
// PWM output on P2.4, P2.5, P2.6, P2.7
// Inputs:  period of P2.4...P2.7 is number of counts before output changes state
//          percentDutyCycle (0 -> 1.0)
//          pin number (1,2,3,4)
// Outputs: none


int TIMER_A0_PWM_Init(uint16_t period, double percentDutyCycle, uint16_t pin)
{

	// Timer A0.1
	if (pin == 1)
	{
		// configure PortPin for pin1 (P2.4) as PWM (1 0) 
		P2->SEL0 |= BIT4; // Select Port2 BIT4 for use as PWM (Set SEL0 = 1)
		P2->SEL1 &= ~BIT4; // Select Port1 BIT4 for use as PWM (Set SEL1 = 0)
		P2->DIR|=BIT4; // set BIT4 for output
	}
    // Timer A0.2
	else if (pin == 2)
	{
		// configure PortPin for pin1 (P2.5) as PWM (1 0) 
		P2->SEL0 |= BIT5; // Select Port2 BIT5 for use as PWM (Set SEL0 = 1)
		P2->SEL1 &= ~BIT5; // Select Port1 BIT5 for use as PWM (Set SEL1 = 0)
		P2->DIR|=BIT5; // set BIT5 for output		
	}	
    // Timer A0.3
	else if (pin == 3)
	{
		// configure PortPin for pin1 (P2.6) as PWM (1 0) 
		P2->SEL0 |= BIT6; // Select Port2 BIT6 for use as PWM (Set SEL0 = 1)
		P2->SEL1 &= ~BIT6; // Select Port1 BIT6 for use as PWM (Set SEL1 = 0)
		P2->DIR|=BIT6; // set BIT6 for output		
	}	
    // Timer A0.4
	else if (pin == 4)
	{
		// configure PortPin for pin1 (P2.7) as PWM (1 0) 
		P2->SEL0 |= BIT7; // Select Port2 BIT7 for use as PWM (Set SEL0 = 1)
		P2->SEL1 &= ~BIT7; // Select Port1 BIT7 for use as PWM (Set SEL1 = 0)
		P2->DIR|=BIT7; // set BIT7 for output		
	}
	else return -2;



	
	// save the period for this timer instance
	// DEFAULT_PERIOD_A0[pin] where pin is the pin number
	DEFAULT_PERIOD_A0[pin] = period;
	// TIMER_A0->CCR[0]
	TIMER_A0->CCR[0] = SystemCoreClock/10000; // #PO_added: generate a ... at 10KHz
	
	

	// TIMER_A0->CCTL[pin]
	TIMER_A0->CCTL[pin] = BIT6;
	
	// set the duty cycle
	uint16_t dutyCycle = (uint16_t) (percentDutyCycle * (double)DEFAULT_PERIOD_A0[pin]);

	// CCR[n] contains the dutyCycle just calculated, where n is the pin number
    //TIMER_A0->CCR[pin]
    TIMER_A0->CCR[pin] = dutyCycle;
	
	// Timer CONTROL register
	// TIMER_A0->CTL
	
	// SMCLK=12MHz, divide by 1, up-down mode
	// bit mode
	// 9-8 10 SMCLK // 7-6 00 ID, divide by 1
	// 5-4 11 MC, up-down mode // BIT 3 Reserved
	// 2 0 TACLR, No clear
	// 1 0 TAIE, no interrupt
	// 0 TAIFG
	TIMER_A0->CTL = BIT5; // #PO_added: just because Ali said most people didnt use Bits 9 and 4
	return 0;
}
//***************************PWM_Duty1*******************************
// change duty cycle of PWM output on pin
// Inputs:  dutycycle, pin
// Outputs: none
// percentDutyCycle is a number between 0 and 1  (ie. 0.5 = 50%)
void TIMER_A0_PWM_DutyCycle(double percentDutyCycle, uint16_t pin)
{
	uint16_t dutyCycle = (uint16_t) (percentDutyCycle * (double)DEFAULT_PERIOD_A0[pin]);
	TIMER_A0->CCR[pin] = dutyCycle;
}

//***************************PWM_Init*******************************
// PWM output on P5.6
// Inputs:  period of P5.6 is number of counts before output changes state
//          percentDutyCycle (0 -> 1.0)//          duty cycle
//          pin number (1,2,3,4), but always 1
// Outputs: none
int TIMER_A2_PWM_Init(uint16_t period, double percentDutyCycle, uint16_t pin)
{

	// NOTE: Timer A2 only exposes 1 PWM pin
	// TimerA2.1
	if (pin == 1)
	{
		// configure PortPin for pin1 (P5.6) as PWM (1 0) 
		P5->SEL0 |= BIT6; // Select Port5 BIT6 for use as PWM (Set SEL0 = 1)
		P5->SEL1 &= ~BIT6; // Select Port5 BIT6 for use as PWM (Set SEL1 = 0)
		P5->DIR|=BIT6; // set BIT6 for output	
	}
	else return -2; 

    // NOTE: Setup similar to TimerA0
    // You will have to use the prescaler (clock divider) to get down to 20ms



	// save the period for this timer instance
	// DEFAULT_PERIOD_A0[pin] where pin is the pin number
	DEFAULT_PERIOD_A0[pin] = period;
	// TIMER_A0->CCR[0]
	TIMER_A0->CCR[0] = SystemCoreClock/10000; // #PO_added: generate a ... at 10KHz
	
	

	// TIMER_A0->CCTL[pin]
	TIMER_A0->CCTL[pin] = BIT6;
	
	// set the duty cycle
	uint16_t dutyCycle = (uint16_t) (percentDutyCycle * (double)DEFAULT_PERIOD_A0[pin]);

	// CCR[n] contains the dutyCycle just calculated, where n is the pin number
    //TIMER_A0->CCR[pin]
    TIMER_A0->CCR[pin] = dutyCycle;
	
	// Timer CONTROL register
	// TIMER_A0->CTL
	
	// SMCLK=12MHz, divide by 1, up-down mode
	// bit mode
	// 9-8 10 SMCLK // 7-6 00 ID, divide by 1
	// 5-4 11 MC, up-down mode // BIT 3 Reserved
	// 2 0 TACLR, No clear
	// 1 0 TAIE, no interrupt
	// 0 TAIFG
	TIMER_A0->CTL = BIT5; // #PO_added: just because Ali said most people didnt use Bits 9 and 4
	return 0;
}
//***************************PWM_Duty1*******************************
// change duty cycle of PWM output on P5.6
// Inputs:  percentDutyCycle, pin  (should always be 1 for TimerA2.1)
//         
// Outputs: none
// 
void TIMER_A2_PWM_DutyCycle(double percentDutyCycle, uint16_t pin)
{

}

