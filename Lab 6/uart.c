/*
 * File:        uart.c
 * Purpose:     Provide UART routines for serial IO
 * Name: 				Kenzo Otsuka
 * Class:				CMPE 460 IDE
 * Lab Section: 2
 * Notes:		
 *
 */
 
#include "Common.h"  // from Lab1 code
#include "msp.h"
#include "uart.h"  // you need to create this file with the function prototypes

#define BAUD_RATE 9600      //default baud rate 
extern uint32_t SystemCoreClock;  // clock rate of MCU

void uart0_init(void){
	//Set the UART to RESET state (set bit0 of EUSCI_A0->CTLW0 register to '1')
	
	EUSCI_A0->CTLW0|=BIT0;

	/*
	bit15=0,      no parity bits
	bit14=x,      not used when parity is disabled
	bit13=0,      LSB first
	bit12=0,      8-bit data length
	bit11=0,      1 stop bit
	bits10-8=000, asynchronous UART mode
	bits7-6=11,   clock source to SMCLK
	bit5=0,       reject erroneous characters and do not set flag
	bit4=0,       do not set flag for break characters
	bit3=0,       not dormant
	bit2=0,       transmit data, not address (not used here)
	bit1=0,       do not transmit break (not used here)
	bit0=1,       hold logic in reset state while configuring
	*/

	// set CTLW0 - hold logic and configure clock source to SMCLK
	EUSCI_A0->CTLW0|=0x80;

	// baud rate
	// N = clock/baud rate = clock_speed/BAUD_RATE
	// set BRW register
	// 3,000,000/9600 =312.5
	EUSCI_A0->BRW|=SystemCoreClock/BAUD_RATE;

	// clear first and second modulation stage bit fields
	// MCTLW register;  
	EUSCI_A0->MCTLW &= ~(UCBRS_M | UCBRF_M | UCOS16);
	

	// P1.3 = TxD
	// P1.2 = RxD
	// we will be using P1.2, P1.3 for RX and TX but not in I/O mode, so we set Port1 SEL1=0 and SEL0=1
	// set SEL0, SEL1 appropriately
	P1->SEL1&=~BIT2;
	P1->SEL1&=~BIT3;
	P1->SEL0|=BIT2;
	P1->SEL0|=BIT3;

	// CTLW0 register - release from reset state
	EUSCI_A0->CTLW0&=~BIT0;

	// disable interrupts (transmit ready, start received, transmit empty, receive full)	
	// IE register;      
	EUSCI_A0->IE &= ~(UCTXCPTIE | UCSTTIE | UCTXIE | UCRXIE);

}

void uart2_init(void){
	//Set the UART to RESET state (set bit0 of EUSCI_A2->CTLW0 register to '1')
	EUSCI_A2->CTLW0|=BIT0;

	/*
	bit15=0,      no parity bits
	bit14=x,      not used when parity is disabled
	bit13=0,      LSB first
	bit12=0,      8-bit data length
	bit11=0,      1 stop bit
	bits10-8=000, asynchronous UART mode
	bits7-6=11,   clock source to SMCLK
	bit5=0,       reject erroneous characters and do not set flag
	bit4=0,       do not set flag for break characters
	bit3=0,       not dormant
	bit2=0,       transmit data, not address (not used here)
	bit1=0,       do not transmit break (not used here)
	bit0=1,       hold logic in reset state while configuring
	*/

	// set CTLW0 - hold logic and configure clock source to SMCLK
	EUSCI_A2->CTLW0|=0x80;

	// baud rate
	// N = clock/baud rate = clock_speed/BAUD_RATE
	// set BRW register
	// New baud rate: 115200? nope...
	// 3,000,000/9600 =312.5
	EUSCI_A2->BRW|=SystemCoreClock/BAUD_RATE;

	// clear first and second modulation stage bit fields
	// MCTLW register;  
	EUSCI_A2->MCTLW &= ~(UCBRS_M | UCBRF_M | UCOS16);
	

	// P1.3 = TxD
	// P1.2 = RxD
	// we will be using P3.2, P3.3 for RX and TX but not in I/O mode, so we set Port3 SEL1=0 and SEL0=1
	// set SEL0, SEL1 appropriately
	P3->SEL1&=~BIT2;
	P3->SEL1&=~BIT3;
	P3->SEL0|=BIT2;
	P3->SEL0|=BIT3;

	// CTLW0 register - release from reset state
	EUSCI_A2->CTLW0&=~BIT0;

	// disable interrupts (transmit ready, start received, transmit empty, receive full)	
	// IE register;      
	EUSCI_A2->IE &= ~(UCTXCPTIE | UCSTTIE | UCTXIE | UCRXIE);
}

char uart0_getchar(void){
	char inChar;
	// Wait for data
	// IFG register
	while((EUSCI_A0->IFG & BIT0) == 0);

	// read character and store in inChar variable
	// RXBUF register
	inChar = (EUSCI_A0->RXBUF);
	

	//Return the 8-bit data from the receiver 
	return(inChar);
}

char uart2_getchar(void){
	char inChar;
	// Wait for data
	// IFG register
	while((EUSCI_A2->IFG & BIT0) == 0);

	// read character and store in inChar variable
	// RXBUF register
	inChar = (EUSCI_A2->RXBUF);
	

	//Return the 8-bit data from the receiver 
	return(inChar);
}

void uart0_putchar(char ch){
	// Wait until transmission of previous bit is complete 
	// IFG register
	while(!(EUSCI_A0->IFG & 0x02));
	
	// send ch character to uart
	// TXBUF register 
	EUSCI_A0->TXBUF = ch;
}

void uart2_putchar(char ch){
	// Wait until transmission of previous bit is complete 
	// IFG register
	while(!(EUSCI_A2->IFG & 0x02));
	
	// send ch character to uart
	// TXBUF register 
	EUSCI_A2->TXBUF = ch;
}

void uart0_put(char *ptr_str){
	while(*ptr_str != 0)
		uart0_putchar(*ptr_str++);
}

void uart2_put(char *ptr_str){
	while(*ptr_str != 0)
		uart2_putchar(*ptr_str++);
}

BOOLEAN uart0_dataAvailable(){
	BOOLEAN go = FALSE;
	// if a character is available, set go = TRUE
	if((EUSCI_A0->IFG & BIT0) != 0){
		go = TRUE;
	}
	return go;
}

BOOLEAN uart2_dataAvailable(){
	BOOLEAN go = FALSE;
	// if a character is available, set go = TRUE
	if((EUSCI_A2->IFG & BIT0) != 0){
		go = TRUE;
	}
	return go;
}

void clearBuffer(char *buffer, int bufferLength){
	int i = 0;
	for (i = 0; i < bufferLength; i++){
		buffer[i] = 0;
	}
}
