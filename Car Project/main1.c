/*
    Main file for car operation
*/

#include <stdint.h>
#include <stdio.h>
#include "msp.h"
#include "ADC14.h"
#include "Common.h"
#include "ControlPins.h"
#include "led.h"
#include "motors.h"
#include "switches.h"
#include "SysTickTimer.h"
#include "Timer32.h"
#include "TimerA.h"
#include "uart.h"
#include "i2c.h"
#include "oled.h"

#define EVER ;;
#define TIMER_A2_PRESCALER_VALUE 64

extern uint16_t line[128];
extern BOOLEAN g_sendData;
char stringBuffer[50] = "";
uint16_t DC_period = 0;
uint16_t servo_period = 0;
int servo_frequency = 50;
uint16_t blackThreshold = 500; //cut-off 
int midpoint = 0;
double servo_duty = 0;
uint16_t servo_pin = 0;
int servo_delay = 150;

/*	Camera_Init function  */
void Camera_Init(void) {
	g_sendData = FALSE;
	ControlPin_SI_Init();
	ControlPin_CLK_Init();
	ADC0_InitSWTriggerCh6();
}

int main(void) {
  LED1_Init();
	LED2_Init();
	Switches_Init();
  uart0_init();
  uart0_put("Car Testing\r\n");
	
  DC_Motor_Init();
  DC_period = SystemCoreClock/10000;
  TIMER_A0_PWM_Init(DC_period, 0, 1);
  TIMER_A0_PWM_Init(DC_period, 0, 2);
  TIMER_A0_PWM_Init(DC_period, 0, 3);
  TIMER_A0_PWM_Init(DC_period, 0, 4);
  DC_Motor_Enable();
  //TIMER_A0_PWM_DutyCycle(0.2, 1);
  
  servo_period = (SystemCoreClock / TIMER_A2_PRESCALER_VALUE)/servo_frequency;
  servo_duty = (1.5/20.0);
  servo_pin = 1;
  if (TIMER_A2_PWM_Init(servo_period, servo_duty, servo_pin) == -2){
    uart0_put("Servo init error\r\n");
    LED1_on();
	}

  OLED_Init();
  sprintf(stringBuffer, "Hello World");
  OLED_Print(1, 1, stringBuffer);

  DisableInterrupts();
  Camera_Init();
  EnableInterrupts();
	
	uart0_put("Entering Loop\r\n");
  int testing = 3;
	if (testing == 1){
		for(EVER){
			if (g_sendData == TRUE) {
				LED1_on();
				// send the array over uart
				sprintf(stringBuffer,"%i\n\r",-1); // start value
				uart0_put(stringBuffer);
				for (int i = 0; i < 128; i++) {
					sprintf(stringBuffer,"%i\n\r", line[i]);
					uart0_put(stringBuffer);
				}
				sprintf(stringBuffer,"%i\n\r",-2); // end value
				uart0_put(stringBuffer);
				LED1_off();
				g_sendData = FALSE;
			}
			// do a small delay
			myDelay(100);
		}
	} else if (testing == 2){
		for(EVER) {
			if (g_sendData == TRUE) {
				// calculate center point
        midpoint = findMidpoint(line);
        OLED_DisplayCameraData(line);

        // part 1: print calculation
        sprintf(stringBuffer, "\r\n Midpoint: %i", midpoint);
        //uart0_put(stringBuffer);
        //printBinAverages(line);
			}
			myDelay(2);
		}
	} else if (testing == 3){
		for(EVER){
			if(g_sendData == TRUE) {
				OLED_DisplayCameraData(line);
				int temp = 0;
				for (int i = 0; i < 128; i++){
					temp += line[i];
				}
				temp = temp/128;
				sprintf(stringBuffer, "%i\r\n", temp);
        myDelay(1);
				uart0_put(stringBuffer);
				LED2_off();
				if(temp < 2000){
					//uart0_put("Low light average\r\n");
					LED2_B_on();
				}
				
				double weightedSumPositions = 0.0;
				double sumOfWeights = 0.0;
				
				for(int i = 0; i < 128; i++){
					weightedSumPositions += line[i] * i;
					sumOfWeights += line[i];
				}
				
				if (sumOfWeights == 0) {
					sumOfWeights = 1;
				}
				
				temp = ( weightedSumPositions / sumOfWeights );
				sprintf(stringBuffer, "%i\r\n", temp);
				myDelay(1);
				uart0_put(stringBuffer);
				
				float servo_sensitivity_modifier = 1.0;
				float servo_pulse = (1.5 / (float)temp) * servo_sensitivity_modifier * 64.0;
				sprintf(stringBuffer, "%f\r\n", servo_pulse);
				myDelay(1);
				uart0_put(stringBuffer);
				float servo_duty = servo_pulse/20.0;
				sprintf(stringBuffer, "%f\r\n\r\n", servo_duty);
				myDelay(1);
				uart0_put(stringBuffer);
				TIMER_A2_PWM_DutyCycle(servo_duty, 1);
				/*
        long unsigned weightSum = 0;
        long unsigned weightedSum = 0;
        long unsigned weightedAverage = 0;
        int scaledWeightAverage = 0;

        for (int i = 0; i < 128; i++) {
          weightedSum += line[i] * (i + 1);
          weightSum += (i + 1);
        }

        weightedAverage = weightedSum / weightSum;

        scaledWeightAverage = (weightedAverage / 16383.0) * 128.0;

        sprintf(stringBuffer, "weightSum: %lu\r\n", weightSum);
        uart0_put(stringBuffer);
        sprintf(stringBuffer, "weightedSum: %lu\r\n", weightedSum);
        uart0_put(stringBuffer);
        sprintf(stringBuffer, "weightedAverage: %lu\r\n", weightedAverage);
        uart0_put(stringBuffer);
        sprintf(stringBuffer, "scaled weight average: %i\r\n\r\n", scaledWeightAverage);
        uart0_put(stringBuffer);
				*/
        /*
				for (int i = 0; i < 128; i++) {
					temp += (line[i] * i);
				}
				temp = temp/16383;
				sprintf(stringBuffer, "temp variable is %i\r\n", temp);
				uart0_put(stringBuffer);
        */
			}
			myDelay(50);
		}
	}	else if (testing == 0){
		uart0_put("How\r\n");
	}
}
