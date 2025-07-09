/*
 * LEDs.c
 *
 *  Created on: Mar 28, 2025
 *      Author: dobao
 */
#include "leds.h"

/*
 * Initialize LEDs
 */
void LED_Green_Init(){
//	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable RCC Clock
	my_RCC_GPIOD_CLK_ENABLE();
	writePin(LED_Green, my_GPIOD, MODER, OUTPUT_MODE); //Set PD12 as output

}

void LED_Orange_Init(){
//	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable RCC Clock
	my_RCC_GPIOD_CLK_ENABLE();
	writePin(LED_Orange, my_GPIOD, MODER, OUTPUT_MODE); //Set PD13 as output

}

void LED_Red_Init(){
//	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable RCC Clock
	my_RCC_GPIOD_CLK_ENABLE();
	writePin(LED_Red, my_GPIOD, MODER, OUTPUT_MODE); //Set PD14 as output
}

void LED_Blue_Init(){
//	__HAL_RCC_GPIOD_CLK_ENABLE(); //Enable RCC Clock
	my_RCC_GPIOD_CLK_ENABLE();
	writePin(LED_Blue, my_GPIOD, MODER, OUTPUT_MODE); //Set PD14 as output
}

/*
 * Control LEDs on or off
 * @param	LED_Color	write LED_Green if you want to toggle the green led
 * @param	on_off		1 is on, 0 is off
 */
void LED_Control(LED_t LED_Color, int on_off){
	if (on_off == 1){ //Turn on the LED
		writePin(LED_Color, my_GPIOD, BSRR, SET); //Set LEDPin of PortD to high
	}
	else{
		writePin(LED_Color, my_GPIOD, BSRR, RESET); //Otherwise turn off the LED
	}
}
