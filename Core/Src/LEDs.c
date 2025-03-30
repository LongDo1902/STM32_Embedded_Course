/*
 * LEDs.c
 *
 *  Created on: Mar 28, 2025
 *      Author: dobao
 */

#include <regBaseAddr.h>
#include "LEDs.h"

// Initialize LEDs pin
void LED3_Init(){ //Set LED3
	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable RCC Clock
	*GPIOD_MODER &= ~(0b11 << 24); //Reset bits 24 to 25
	*GPIOD_MODER |= (0b01 << 24); //set PD12 to output mode
}

void LED4_Init(){ //Set LED4
	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable RCC Clock
	*GPIOD_MODER &= ~(0b11 << 26); //Reset bits 26 to 27
	*GPIOD_MODER |= (0b01 << 26); //set PD13 to output mode
}

void LED5_Init(){ //Set LED5
	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable RCC Clock
	*GPIOD_MODER &= ~(0b11 << 28); //Reset bits 28 to 29
	*GPIOD_MODER |= (0b01 << 28); //set PD14 to output mode
}

void LED6_Init(){ //Set LED6
	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable RCC Clock
	*GPIOD_MODER &= ~(0b11 << 30); //Reset bits 30 to 31
	*GPIOD_MODER |= (0b01 << 30); //set PD15 to output mode
}

void LEDs_Init(){ //Init all 4 LEDs
	LED3_Init();
	LED4_Init();
	LED5_Init();
	LED6_Init();
}

//Activate LEDs
void LED_Control(uint8_t pin, int on_off){
	if (on_off == 1){ //Turn on the LED
		*GPIOD_BSRR = (1 << pin);
	}
	else{
		*GPIOD_BSRR = (1 << (pin + 16)); //Otherwise turn off the LED
	}
}
