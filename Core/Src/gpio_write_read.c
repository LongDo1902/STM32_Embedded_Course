/*
 * gpio_write_read.c
 *
 *  Created on: Apr 13, 2025
 *      Author: dobao
 */

#include <stdio.h>
#include <stdint.h>
#include "gpio_write_read.h"
#include "registerAddress.h"

/*
 * Helper function to write bit to a pin to a GPIO on STM32F411
 * For example, want to write GPIO A, write my_GPIOA to "port"
 * ..., want to write pin 1, write my_GPIO_PIN_1 to "pin"
 * ..., want to pull pin low, write my_GPIO_PIN_RESET to "state"
 */
void GPIO_WritePin(GPIO_portName port, GPIO_pin pin, uint8_t state){
	GPIO_Register_Offset_t* GPIOx; //Create a pointer that points to memory layout
	switch(port){
		case my_GPIOA: GPIOx = GPIOA_REG; break;
		case my_GPIOB: GPIOx = GPIOB_REG; break;
		case my_GPIOC: GPIOx = GPIOC_REG; break;
		case my_GPIOD: GPIOx = GPIOD_REG; break;
		case my_GPIOE: GPIOx = GPIOE_REG; break;
		case my_GPIOH: GPIOx = GPIOH_REG; break;
		default: return; //Invalid port!
	}
	if (state == my_GPIO_PIN_RESET){
		GPIOx -> BSRR = (1 << (pin+16));
	} else if(state == my_GPIO_PIN_SET){
		GPIOx -> BSRR = (1 << pin);
	} else{
		return; //Invalid state
	}
}

uint8_t GPIO_ReadPin(GPIO_portName port, GPIO_pin pin){
	GPIO_Register_Offset_t* GPIOx;
	switch(port){
		case my_GPIOA: GPIOx = GPIOA_REG; break;
		case my_GPIOB: GPIOx = GPIOB_REG; break;
		case my_GPIOC: GPIOx = GPIOC_REG; break;
		case my_GPIOD: GPIOx = GPIOD_REG; break;
		case my_GPIOE: GPIOx = GPIOE_REG; break;
		case my_GPIOH: GPIOx = GPIOH_REG; break;
		default: return 0; //Invalid port!
	}
	//(condition) ? value_if_true : value_if_false;
	return (GPIOx -> IDR & (1 << pin)) ? 1 : 0;
}










