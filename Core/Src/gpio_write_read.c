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
 * Helper function to write bit to a pin to a GPIO
 * @param port is my_GPIOA if it is port A
 * @param pinNum is pin number write my_GPIO_PIN_1 if pin is 1
 * @param state is high or low state of the pin
 */
void GPIO_WritePin(GPIO_pin_t pinNum, GPIO_portName_t port, GPIO_State_t state, GPIO_Mode_t mode){

	//Pick GPIOx base
	GPIO_Register_Offset_t* GPIOx; //Create a pointer that points to memory layout
	switch(port){
		case my_GPIOA: GPIOx = GPIOA_REG; break; //Assign pointer GPIOx to pointer GPIOA_REG
		case my_GPIOB: GPIOx = GPIOB_REG; break;
		case my_GPIOC: GPIOx = GPIOC_REG; break;
		case my_GPIOD: GPIOx = GPIOD_REG; break;
		case my_GPIOE: GPIOx = GPIOE_REG; break;
		case my_GPIOH: GPIOx = GPIOH_REG; break;
		default: return; //Invalid port!
	}

	//Handle BSRR as a special write only case
	if(mode == BSRR){
		uint32_t mask = (state == my_GPIO_PIN_SET)
						?(1U << pinNum)			//set bit
						:(1U << (pinNum + 16)); //reset bit
		GPIOx -> BSRR = mask; //atomic
		return;
	}

	//For all the rest, pick reg pointer and how many bits to shift
	volatile uint32_t *reg; //pointer reg
	uint32_t bitShift;
	uint32_t bitWidth; //field width: 1 bit for OTYPER, else 2 bits

	switch(mode){
		case OTYPER:
			reg = &GPIOx -> OTYPER;
			bitShift = pinNum;
			bitWidth = 1;
			break;

		case MODER:
			reg = &GPIOx -> MODER;
			bitShift = pinNum * 2; //jump 2*pinNum bits from the LSB to get to that pin's field
			bitWidth = 2;
			break;

		case OSPEEDR:
			reg = &GPIOx -> OSPEEDR;
			bitShift = pinNum * 2;
			bitWidth = 2;
			break;

		case PUPDR:
			reg = &GPIOx -> PUPDR;
			bitShift = pinNum * 2;
			bitWidth = 2;
			break;

		default: return;
	}

	//Mask off old bits, OR in new state
	uint32_t mask = ((1U << bitWidth) - 1U) << bitShift;
	uint32_t value = ((uint32_t)state << bitShift) & mask; //& mask zeroes out anything that accidentally spilled outside the field
	*reg = (*reg & ~mask) | value; //Clear the old bit before OR with value
}



//
//uint8_t GPIO_ReadPin(GPIO_portName port, GPIO_pin pin){
//	GPIO_Register_Offset_t* GPIOx;
//	switch(port){
//		case my_GPIOA: GPIOx = GPIOA_REG; break;
//		case my_GPIOB: GPIOx = GPIOB_REG; break;
//		case my_GPIOC: GPIOx = GPIOC_REG; break;
//		case my_GPIOD: GPIOx = GPIOD_REG; break;
//		case my_GPIOE: GPIOx = GPIOE_REG; break;
//		case my_GPIOH: GPIOx = GPIOH_REG; break;
//		default: return 0; //Invalid port!
//	}
//	//(condition) ? value_if_true : value_if_false;
//	return (GPIOx -> IDR & (1 << pin)) ? 1 : 0;
//}










