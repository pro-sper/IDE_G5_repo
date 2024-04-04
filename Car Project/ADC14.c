// ADC14.c
// Runs on MSP432
// ADC input, software trigger, 14-bit conversion,
// 2.5 V static (always on) reference
// Daniel Valvano
// June 11, 2015

// Modified
// LJBeato
// March 2021

#include <stdint.h>
#include "msp.h"
#include "Common.h"
#include "CortexM.h"
#include "ADC14.h"
#include <stdio.h>
#include <string.h>

#define LINE_SIZE 128
#define BIN_COUNT 16
#define BIN_SIZE ( LINE_SIZE / BIN_COUNT )

void printBinAverages(uint16_t line[]) {
    uint16_t binAverages[BIN_COUNT] = {0};
    char binString[200]; // Ensure this string is large enough to hold all numbers
    char temp[16]; // Temporary string for number conversion
	int bin = 0;
	int i = 0;
    
    for (bin = 0; bin < BIN_COUNT; bin++) {
        uint32_t sum = 0;
        for (i = 0; i < BIN_SIZE; i++) {
            sum += line[bin * BIN_SIZE + i];
        }
        binAverages[bin] = sum / BIN_SIZE;
    }
    
    // Construct the string to print
    for (i = 0; i < BIN_COUNT; i++) {
        sprintf(temp, "%u ", binAverages[i]);
        strcat(binString, temp);
    }
    
    printf("Bin Averages: %s\n", binString);
}

void smoothData(uint16_t line[]) {
    uint16_t temp[LINE_SIZE];
	int i = 0;
    // Apply smoothing only to indices 2 through LINE_SIZE - 3
    temp[0] = line[0];
    temp[1] = line[1];
    for (i = 2; i < LINE_SIZE - 2; i++) {
        temp[i] = (line[i - 2] + line[i - 1] + line[i] + line[i + 1] + line[i + 2]) / 5;
    }
    temp[LINE_SIZE - 2] = line[LINE_SIZE - 2];
    temp[LINE_SIZE - 1] = line[LINE_SIZE - 1];
    
    // Copy smoothed data back to original array
    for (i = 0; i < LINE_SIZE; i++) {
        line[i] = temp[i];
    }
}

int findMidpoint(uint16_t line[]) {
	uint16_t average = 0;
	uint16_t blackThreshold = 0;
	int firstBlackBarStart = 0;
	int firstBlackBarEnd = 0;
	int secondBlackBarStart = 0;
	int secondBlackBarEnd = 0;
    // Calculate the average value of the array
    uint32_t sum = 0;
	int i = 0;
    for (i = 0; i < LINE_SIZE; i++) {
        sum += line[i];
    }
    average = sum / LINE_SIZE;
    
    // Define blackThreshold as 0.4 times the average value
    blackThreshold = (uint16_t)(0.4 * average);

    // Smooth the data
    smoothData(line);

    // Placeholder for the logic to identify black bars and calculate the midpoint
    // This part of the code should now use the dynamically calculated blackThreshold
    
    // Initialize variables to track the black bars
    firstBlackBarStart = -1;
	firstBlackBarEnd = -1;
    secondBlackBarStart = -1;
	secondBlackBarEnd = -1;
    
    // Iterate through the smoothed data to identify the black bars
    for (i = 0; i < LINE_SIZE; i++) {
        if (line[i] < blackThreshold) {
            if (firstBlackBarEnd == -1) {
                if (firstBlackBarStart == -1) firstBlackBarStart = i;
                firstBlackBarEnd = i;
            } else if (secondBlackBarStart == -1 || i - secondBlackBarEnd < 5) {
                if (secondBlackBarStart == -1) secondBlackBarStart = i;
                secondBlackBarEnd = i;
            }
        }
    }
    
    // Check if two black bars were identified and calculate the midpoint
    if (firstBlackBarStart != -1 && secondBlackBarStart != -1) {
        return (firstBlackBarEnd + secondBlackBarStart) / 2;
    }
    
    // Return an error signal if two black bars were not found
    return -1;
}

// We are going to use Port 4 Pin 7 which is ADC A6
// P4.7 = A6
void ADC0_InitSWTriggerCh6(void) {
	// wait for reference to be idle
	// REF_A->CTL0
	while((REF_A->CTL0&BIT(12))==1){};

	// set reference voltage to 2.5V
	// 1) configure reference for static 2.5V
	// REF_A->CTL0
  	REF_A->CTL0 = 0x0039;
		
	// wait for reference voltage to be ready
	// REF_A->CTL0
  	while((REF_A->CTL0&0x1000) == 0){};

	// 2) ADC14ENC = 0 to allow programming
	// ADC14->CTL0
  	ADC14->CTL0 = 0;

	// 3) wait for BUSY to be zero
	// ADC14->CTL0
	while(ADC14->CTL0&0x00010000){};


	// ------------------------------------------------------------------
	// 31-30 ADC14PDIV  predivider,            00b = Predivide by 1
	// 29-27 ADC14SHSx  SHM source            000b = ADC14SC bit
	// 26    ADC14SHP   SHM pulse-mode          1b = SAMPCON the sampling timer
	// 25    ADC14ISSH  invert sample-and-hold  0b =  not inverted
	// 24-22 ADC14DIVx  clock divider         000b = /1
	// 21-19 ADC14SSELx clock source select   100b = SMCLK
	// 18-17 ADC14CONSEQx mode select          00b = Single-channel, single-conversion
	// 16    ADC14BUSY  ADC14 busy              0b (read only)

	// 15-12 ADC14SHT1x sample-and-hold time 0011b = 32 clocks (use 32 clocks)

	// 11-8  ADC14SHT0x sample-and-hold time 0011b = 32 clocks (use 32 clocks)

	// 7     ADC14MSC   multiple sample         0b = not multiple
	// 6-5   reserved                          00b (reserved)
	// 4     ADC14ON    ADC14 on                1b = powered up

	// 3-2   reserved                          00b (reserved)
	// 1     ADC14ENC   enable conversion       0b = ADC14 disabled
	// 0     ADC14SC    ADC14 start             0b = No start (yet)
	// ------------------------------------------------------------------
	// 4) single, SMCLK, on, disabled, /1, 32 clocks, SHM	pulse-mode
	// ADC14->CTL0
	ADC14->CTL0 &= ~BIT(18);	// single
	ADC14->CTL0 &= ~BIT(17);	// single

	ADC14->CTL0 |=  BIT(21);	// SMCLK
	ADC14->CTL0 &= ~BIT(20);	// SMCLK
	ADC14->CTL0 &= ~BIT(19);	// SMCLK

	ADC14->CTL0 |=  BIT(4); 	// on

	ADC14->CTL0 &= ~BIT(1); 	// disabled

	ADC14->CTL0 &= ~BIT(24);	// /1
	ADC14->CTL0 &= ~BIT(23);	// /1
	ADC14->CTL0 &= ~BIT(22);	// /1

	ADC14->CTL0 &= ~BIT(15);	// 32 clocks
	ADC14->CTL0 &= ~BIT(14);	// 32 clocks
	ADC14->CTL0 |=  BIT(13);	// 32 clocks
	ADC14->CTL0 |=  BIT(12);	// 32 clocks
	ADC14->CTL0 &= ~BIT(11);	// 32 clocks
	ADC14->CTL0 &= ~BIT(10);	// 32 clocks
	ADC14->CTL0 |=  BIT(9); 	// 32 clocks
	ADC14->CTL0 |=  BIT(8); 	// 32 clocks

	ADC14->CTL0 |=  BIT(26);	// SHM pulse-mode



	// 20-16 STARTADDx  start addr          00000b = ADC14MEM0
	// 15-6  reserved                  0000000000b (reserved)
	// --
	// 5-4   ADC14RES   ADC14 resolution       11b = 14 bit, 16 clocks
	// --
	// 3     ADC14DF    data read-back format   0b = Binary unsigned
	// 2     REFBURST   reference buffer burst  0b = reference on continuously
	// 1-0   ADC14PWRMD ADC power modes        00b = Regular power mode
	//
	// 5) ADC14MEM0, 14-bit, ref on, regular power
	// ADC14->CTL1

  	ADC14->CTL1 |=  BIT(5);	// 14-bit
  	ADC14->CTL1 |=  BIT(4);	// 14-bit

	ADC14->CTL1 &= ~BIT(2); // ref on

	ADC14->CTL1 &= ~BIT(1); // regular power
	ADC14->CTL1 &= ~BIT(0); // regular power

	// ADC14->MCTL[0]
	// VREF buffered
	// End of sequence
	// 00110b = If ADC14DIF = 0: A6;
	// // 6) 0 to 2.5V, channel 6
	ADC14->MCTL[0] = 0x00000186;


	// 15   ADC14WINCTH Window comp threshold   0b = not used
	// 14   ADC14WINC   Comparator enable       0b = Comparator disabled
	// 13   ADC14DIF    Differential mode       0b = Single-ended mode enabled
	// 12   reserved                            0b (reserved)
	// 11-8 ADC14VRSEL  V(R+) and V(R-)      0001b = V(R+) = VREF, V(R-) = AVSS
	// 7    ADC14EOS    End of sequence         1b = End of sequence
	// 6-5  reserved                           00b (reserved)
	// 4-0  ADC14INCHx  Input channel        0110b = A6, P4.7

	// 7) no interrupts
	// ADC14->IER0
	// ADC14->IER1
	ADC14->IER0 = 0;
	ADC14->IER1 = 0;                     // no interrupts
	//
	// P4.7 is Analog In A6
	// 8) analog mode on A6, P4.7
	// set pins for ADC A6
	// SEL0, SEL1
	P4->SEL0 |= BIT(7);
	P4->SEL1 |= BIT(7);

	// 9) enable
	// ADC14->CTL0
	ADC14->CTL0 |= BIT(1);  	// disabled
}


// ADC14->IFGR0 bit 0 is set when conversion done
// cleared on read ADC14MEM0
// ADC14->CLRIFGR0 bit 0, write 1 to clear flag
// ADC14->IVx is 0x0C when ADC14MEM0 interrupt flag; Interrupt Flag: ADC14IFG0
// ADC14->MEM[0] 14-bit conversion in bits 13-0 (31-16 undefined, 15-14 zero)
unsigned int  ADC_In(void){
	unsigned int adcIn;

	// 1) wait for BUSY to be zero  ADC14->CTL0
	// ADC14->CTL0
	while(ADC14->CTL0&0x00010000){}; 
		
	// 2) start single conversion	  
	// ADC14->CTL0
	ADC14->CTL0 |= BIT(0);

	// 3) wait for ADC14->IFGR0, ADC14->IFGR0 bit 0 is set when conversion done
	// ADC14->IFGR0
	while((ADC14->IFGR0&BIT(0))==0){};
		
	// 14 bit sample returned  ADC14->MEM[0]
	// ADC14->MEM[0] 14-bit conversion in bits 13-0 (31-16 undefined, 15-14 zero)
	// ADC14->MEM[0]
	adcIn = ADC14->MEM[0];
		
	return adcIn;                 // 4) return result 0 to 16383
}
