/*
 * buttons.c
 *
 *  Created on: Mar 30, 2025
 *      Author: dobao
 */
#include "buttons.h"
#include "LEDs.h"

void buttonB1Init(){ //Initialize the button
	__HAL_RCC_GPIOA_CLK_ENABLE(); //Enable RCC Clock
	*GPIOA_MODER &= ~(0b11 << 0); //Reset bits 0 and 1 of MODER
	*GPIOA_MODER |= (0b00 << 0); //Set the pin A0 as input

	*GPIOA_PUPDR &= ~(0b11 << 0); //Reset bits 0 and 1 of PUPDR
	*GPIOA_PUPDR |= (0b10 << 0); //Set this pin as a pull-down config
}

void buttonControl(){ //Control the button state
	while(*GPIOA_IDR & 0x01){ //check if PA0 is high
		//Button pressed
		LED_Control(LED3_PIN, 1);
		HAL_Delay(100);

		LED_Control(LED4_PIN, 1);
		HAL_Delay(100);

		LED_Control(LED5_PIN, 1);
		HAL_Delay(100);

		LED_Control(LED6_PIN, 1);
		HAL_Delay(100);

		//Turn off all the LEDs before a new cycle
		LED_Control(LED3_PIN, 0);
		LED_Control(LED4_PIN, 0);
		LED_Control(LED5_PIN, 0);
		LED_Control(LED6_PIN, 0);
		HAL_Delay(100);
	}

	//Butotn not pressed
		LED_Control(LED3_PIN, 0);
		LED_Control(LED4_PIN, 0);
		LED_Control(LED5_PIN, 0);
		LED_Control(LED6_PIN, 0);
}
