/* 
Title: Lab5: switches.c 
Purpose: Houses switch press logic
Name: Prosper Omiponle  
Date: 2/23/2024

// NEEDS initialization for IFG stuff
  
*/
#include "msp.h" 

#include "Common.h"

#include "switches.h"

void Switch1_Init(void){
	// configure PortPin for Switch 1 and Switch2 as port I/O 
	P1->SEL0 &= ~BIT1;		//Switch 1 P1.1
	P1->SEL1 &= ~BIT1;

	// configure as input
	P1->DIR &= ~BIT1;
	P1->REN |= BIT1;
	P1->OUT |= BIT1;

	// Selecting the edge which will trigger the interrupt
	P1->IES |= BIT1; // #PO_added: because P1.1
	P1->IFG &= ~BIT1;
	P1->IE |= BIT1;
}

void Switch2_Init(void){
	P1->SEL0 &= ~BIT4;		//Switch 2 P1.4
	P1->SEL1 &= ~BIT4;

	P1->DIR &= ~BIT4;
	P1->REN |= BIT4;
	P1->OUT |= BIT4;

	P1->IES |= BIT4; // #PO_added: because P1.4
	P1->IFG &= ~BIT4;	
	P1->IE |= BIT4;
}

void Switches_Init(void)
{
	// configure PortPin for Switch 1 and Switch2 as port I/O 
	P1->SEL0 &= ~BIT1;		//Switch 1 P1.1
	P1->SEL1 &= ~BIT1;

	P1->SEL0 &= ~BIT4;		//Switch 2 P1.4
	P1->SEL1 &= ~BIT4;

	// configure as input
	P1->DIR &= ~BIT1;
	P1->REN |= BIT1;
	P1->OUT |= BIT1;

	P1->DIR &= ~BIT4;
	P1->REN |= BIT4;
	P1->OUT |= BIT4;

	// Selecting the edge which will trigger the interrupt
	P1->IES |= BIT1; // #PO_added: because P1.1
	P1->IES |= BIT4; // #PO_added: because P1.4

	// Clear the pin's interrupt flag
	P1->IFG &= ~BIT1;
	P1->IFG &= ~BIT4;

	//Set Interupt Enable (IE) bit for our specific pin
	P1->IE |= BIT1;
	P1->IE |= BIT4;
}
//------------Switch_Input------------
// Read and return the status of Switch1
// Input: none
// return: TRUE if pressed
//         FALSE if not pressed
BOOLEAN Switch1_Pressed(void)
{
	BOOLEAN retVal = FALSE;
	// check if pressed
	if ((P1->IN & BIT(1)) != BIT(1)){	//pressed
		retVal = TRUE;
	}
	return (retVal);              // return TRUE(pressed) or FALSE(not pressed)
}
//------------Switch_Input------------
// Read and return the status of Switch2
// Input: none
// return: TRUE if pressed
//         FALSE if not pressed
BOOLEAN Switch2_Pressed(void)
{
	BOOLEAN retVal = FALSE;
	// check if pressed
	if ((P1->IN & BIT(4)) != BIT(4)) {
		retVal = TRUE;
	}
	return (retVal);              // return TRUE(pressed) or FALSE(not pressed)
}
