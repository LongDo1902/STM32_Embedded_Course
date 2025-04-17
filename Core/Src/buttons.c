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
 * @param buttonPin is a pin number that is connected to a button
 */
void buttonInit(char buttonPin, GPIO_portName_t portName){
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_WritePin(0, portName, mode_0x00, MODER); //Set buttonPin of portName as input
	GPIO_WritePin(0, portName, mode_0x02, PUPDR); //Set buttonPin of portName as pull-down config
}

char buttonState(){
	return (GPIOA_REG -> IDR & 0x01); //return 1 for high and 0 for low
}

void buttonInterrupt(){
	if(buttonState()){
		//The button is pressed
		LED_Control(LED_Green, 1);
	}
	else{
		LED_Control(LED_Green, 0);
	}
}
