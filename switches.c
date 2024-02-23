/* 
Title: Lab5: switches.c 
Purpose: Houses switch press logic
Name: Prosper Omiponle  
Date: 2/23/2024

This file prepared by LJBeato
01/11/2021
  
*/
#include "msp.h" 

#include "Common.h"

#include "switches.h"


void Switches_Init(void)
{
	// configure PortPin for Switch 1 and Switch2 as port I/O 
	P1->SEL0 &= ~BIT1;		//Switch 1 P1.1
	P1->SEL1 &= ~BIT1;
	
	P1->SEL0 &= ~BIT4;		//Switch 1 P1.4
	P1->SEL1 &= ~BIT4;
	
	// configure as input
	P1->DIR &= ~BIT1;
	P1->REN |= BIT1;
	P1->OUT |= BIT1;
	
	P1->DIR &= ~BIT4;
	P1->REN |= BIT4;
	P1->OUT |= BIT4;
              
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
	if ((P1->IN & BIT1) != BIT1){	//pressed
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
	if ((P1->IN & BIT4) != BIT4){
		retVal = TRUE;
	}
	return (retVal);              // return TRUE(pressed) or FALSE(not pressed)
}


void delay(int i){
	while(i > 0){
		i--;
	}
}
