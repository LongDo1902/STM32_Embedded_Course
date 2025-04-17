/*
 * LEDs.c
 *
 *  Created on: Mar 28, 2025
 *      Author: dobao
 */

#include "registerAddress.h"
#include "LEDs.h"

//Initialize LEDs
void LED_Green_Init(){
	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable RCC Clock
	GPIO_WritePin(12, my_GPIOD, mode_0x01, MODER); //Set PD12 as output

}

void LED_Orange_Init(){
	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable RCC Clock
	GPIO_WritePin(13, my_GPIOD, mode_0x01, MODER); //Set PD13 as output

}

void LED_Red_Init(){
	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable RCC Clock
	GPIO_WritePin(14, my_GPIOD, mode_0x01, MODER); //Set PD14 as output
}

void LED_Blue_Init(){
	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable RCC Clock
	GPIO_WritePin(15, my_GPIOD, mode_0x01, MODER); //Set PD14 as output
}

//Activate LEDs
void LED_Control(LED_t LED_Color, int on_off){
	if (on_off == 1){ //Turn on the LED
		GPIO_WritePin(LED_Color, my_GPIOD, my_GPIO_PIN_SET, BSRR); //Set LEDPin of PortD to high
	}
	else{
		GPIO_WritePin(LED_Color, my_GPIOD, my_GPIO_PIN_RESET, BSRR); //Otherwise turn off the LED
	}
}
