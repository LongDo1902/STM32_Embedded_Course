/*
 * buttons.c
 *
 *  Created on: Mar 30, 2025
 *      Author: dobao
 */
#include "buttons.h"
#include "LEDs.h"
#include "registerAddress.h"
#include "gpio_write_read.h"

/*
 * A function to initialize any chosen button
 * @param 	buttonPin 	a pin number that is connected to a button
 * @param	portName 	connected Port such as my_GPIOA
 */
void buttonInit(char buttonPin, GPIO_PortName_t portName){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_WritePin(buttonPin, portName, MODER, mode_00); //Set buttonPin of portName as an input mode
	GPIO_WritePin(buttonPin, portName, PUPDR, mode_02); //Set buttonPin of portName as pull-down config
}

/*
 * A helper function return 1 if there is a signal from the button, otherwise return 0
 */
char buttonState(){
	return (GPIOA_REG -> IDR & 0x01);
}

/*
 * A function turns the Green LED on when the button is pressed, otherwise nothing
 */
void buttonInterrupt(){
	if(buttonState()){
		//The button is pressed
		LED_Control(LED_Green, 1);
	}
	else{
		LED_Control(LED_Green, 0);
	}
}
