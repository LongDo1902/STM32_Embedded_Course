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
 * Helper function to write bit to a pin to a GPIO for general purposes
 * @param 	port	my_GPIOA if it is port A
 * @param 	pinNum	pin number write my_GPIO_PIN_1 if pin is 1
 * @param 	state	my_GPIO_PIN_SET if the pin is high
 *
 * @Info: see gpio_write_read.h
 */
void GPIO_WritePin(GPIO_Pin_t pinNum, GPIO_PortName_t port, GPIO_Mode_t mode, GPIO_State_t state){
	if(pinNum > 15) return; //safeguard

	//Create pointer GPIOx which has connection to GPIO_Register_Offset_t
	GPIO_Register_Offset_t* GPIOx;

	switch(port){
		case my_GPIOA: GPIOx = GPIOA_REG; break; //Assign pointer GPIOx to pointer GPIOA_REG
		case my_GPIOB: GPIOx = GPIOB_REG; break;
		case my_GPIOC: GPIOx = GPIOC_REG; break;
		case my_GPIOD: GPIOx = GPIOD_REG; break;
		case my_GPIOE: GPIOx = GPIOE_REG; break;
		case my_GPIOH: GPIOx = GPIOH_REG; break;
		default: return; //Invalid port!
	}

	//Handle BSRR as special write only case
	if(mode == BSRR){
		uint32_t mask = (state == my_GPIO_PIN_SET)
						?(1U << pinNum)			//set
						:(1U << (pinNum + 16)); //reset
		GPIOx -> BSRR = mask;
		return;
	}

	volatile uint32_t *reg; //A pointer to store the address and access to the memory space at that address
	uint32_t bitWidth; //number of bits
	uint32_t bitShift; //number of bits to shift

	switch(mode){
		case OTYPER:
			reg = &GPIOx -> OTYPER;
			bitWidth = 1;
			bitShift = pinNum * bitWidth;
			break;

		case MODER:
			reg = &GPIOx -> MODER;
			bitWidth = 2;
			bitShift = pinNum * bitWidth;
			break;

		case OSPEEDR:
			reg = &GPIOx -> OSPEEDR;
			bitWidth = 2;
			bitShift = pinNum * bitWidth;
			break;

		case PUPDR:
			reg = &GPIOx -> PUPDR;
			bitWidth = 2;
			bitShift = pinNum * bitWidth;
			break;

		case AFRL:
			if(pinNum > 7) return;
			reg = &GPIOx -> AFRL;
			bitWidth = 4;
			bitShift = pinNum * bitWidth;
			break;

		case AFRH:
			if(pinNum < 8) return; //Invalid pin
			reg = &GPIOx -> AFRH;
			bitWidth = 4;
			bitShift = (pinNum - 8) * bitWidth;
			break;

		default: return;
	}

	//Mask off old bits OR in new state
	uint32_t mask = ((1U << bitWidth) - 1U) << bitShift;
	uint32_t value = ((uint32_t)state << bitShift) & mask; //& mask to zero out anything that accidentally spilled outside the field
	*reg = (*reg & ~mask) | value; //Clear the old but before OR with value
}


/*
 * Helper function to write bit to pin for EXTI purpose
 * @param 	bitPosition:	the bit location that you want to write
 * @param 	mode:			IMR if Interrupt Mask Reg is selected
 * @param 	state:			Set or Ret
 *
 * @Info: See gpio_write_read.h
 */
void GPIO_WriteEXTI(uint8_t bitPosition, EXTI_Mode_t mode, GPIO_State_t state){
	volatile uint32_t *reg; //A pointer to store the address and access to the memory space at that address
	uint32_t bitShift = bitPosition; //Position that we want to wrote the value (bit) to

	switch(mode){
		case IMR:
			reg = &EXTI_REG -> IMR;
			break;

		case EMR:
			reg = &EXTI_REG -> EMR;
			break;

		case RTSR:
			reg = &EXTI_REG -> RTSR;
			break;

		case FTSR:
			reg = &EXTI_REG -> FTSR;
			break;

		case SWIER:
			reg = &EXTI_REG -> SWIER;
			break;

		case PR:
			reg = &EXTI_REG -> PR;
			break;

		default: return; //Invalid mode
	}

	//Mask off the old bit and OR with new value
	uint32_t mask = ((1U << 1) - 1U) << bitShift;
	uint32_t value = ((uint32_t)state << bitShift) & mask;
	*reg = (*reg & ~mask) | value;
}



/*
 * Helper function to write bit to pins to config UART
 * @param	bitPosition		bit location that you want to write
 * @param	userUARTx		write my_UART1 if want to write UART1
 * @param	mode			different UART mode registers
 * @param	state			write set for 1, reset for 0
 */
void GPIO_WriteUART(uint8_t bitPosition, UART_Name_t userUARTx, UART_Mode_t mode, uint32_t value){
	UART_Register_Offset_t* UARTx;
	switch(userUARTx){
		case my_UART1: UARTx = UART1_REG; break;
		case my_UART2: UARTx = UART2_REG; break;
		case my_UART6: UARTx = UART6_REG; break;
		default: return;
	}

	volatile uint32_t* reg;
	switch(mode){
		case DR: reg = &UARTx -> DR; break;
		case BRR: reg = &UARTx -> BRR; break;
		case CR1: reg = &UARTx -> CR1; break;
		case CR2: reg = &UARTx -> CR2; break;
		case CR3: reg = &UARTx -> CR3; break;
		case GTPR: reg = &UARTx -> GTPR; break;
		default: return;
	}

	//Auto detect bitwidth based on the "value" length
	uint8_t bitWidth = 0;
	uint16_t temp = value;
	while(temp > 0){
		bitWidth++; //Increment the bidWidth by one when ever temp > 0
		temp = temp >> 1; //Shift the temp to the right by 1
	}

	//Mask off the old bit and OR with new value
	uint32_t mask = ((1U << bitWidth) - 1U) << bitPosition;
	uint32_t shiftedValue = (value << bitPosition) & mask;
	*reg = (*reg & ~mask) | shiftedValue;
}



/*
 * Helper function to read the pin's status
 * 	@param
 * 	@param
 *
 * 	returns:
 *		1 if the specified bit is set (bit = 1)
 *		0 if the soecified bit is cleared (bit = 0)
 *		-1 if an invalid port/pin is selected or register mode is selected
 */
char readPin(uint8_t bitPosition, GPIO_PortName_t port, GPIO_Mode_t mode){
	//Validate bit position (only 0 - 15 are valid for GPIO pins)
	if(bitPosition > 15) return -1;

	//Create a pointer GPIOx which has connection to GPIO_Register_Offset_t
	GPIO_Register_Offset_t* GPIOx;

	switch(port){
		case my_GPIOA: GPIOx = GPIOA_REG; break;
		case my_GPIOB: GPIOx = GPIOB_REG; break;
		case my_GPIOC: GPIOx = GPIOC_REG; break;
		case my_GPIOD: GPIOx = GPIOD_REG; break;
		case my_GPIOE: GPIOx = GPIOE_REG; break;
		case my_GPIOH: GPIOx = GPIOH_REG; break;
		default: return -1; //Invalid port
	}

	volatile uint32_t* reg;
	switch(mode){
		case MODER: 	reg = &GPIOx -> MODER; 	break;		//RW
		case OTYPER: 	reg = &GPIOx -> OTYPER; break;		//RW
		case OSPEEDR: 	reg = &GPIOx -> OSPEEDR; break;		//RW
		case PUPDR: 	reg = &GPIOx -> PUPDR; break;		//RW
		case IDR: 		reg = &GPIOx -> IDR; break;			//R
		case ODR:		reg = &GPIOx -> ODR; break;			//RW
		default: return -1; //Invalid port
	}

	//Always read the full register and mask the bit you need
	return ((*reg >> bitPosition) & 0x1);
}



/*
 * Reads the status of a specific bit from a UART peripheral register.
 *
 *  @param	bitPosition	Bit position to read (0-31).
 *  @param	userUARTx	UART peripheral to access (e.g., my_UART1, my_UART2, my_UART6).
 *  @param	mode		UART register to read from (e.g., SR, DR, CR1, etc.).
 *
 * Returns:
 *  	1 if the specified bit is set (bit = 1).
 *  	0 if the specified bit is cleared (bit = 0).
 *  	-1 if an invalid UART peripheral or register mode is selected.
 */
char readUART(uint8_t bitPosition, UART_Name_t userUARTx, UART_Mode_t mode){
	UART_Register_Offset_t* UARTx;
	switch(userUARTx){
		case my_UART1: UARTx = UART1_REG; break;
		case my_UART2: UARTx = UART2_REG; break;
		case my_UART6: UARTx = UART6_REG; break;
		default: return -1; //return an error value
	}

	volatile uint32_t* reg;
	switch(mode){
		case SR: reg = &UARTx -> SR; break; //R
		case DR: reg = &UARTx -> DR; break; //RW
		case BRR: reg = &UARTx -> BRR; break; //RW
		case CR1: reg = &UARTx -> CR1; break; //RW
		case CR2: reg = &UARTx -> CR2; break; //RW
		case CR3: reg = &UARTx -> CR3; break; //RW
		case GTPR: reg = &UARTx -> GTPR; break; //RW
		default: return -1;
	}

	//Different mode has different behavior
	if (mode == DR){
		return (char)(*reg & 0xFF);
	}

	else{
		if(((*reg) >> bitPosition) & 0x1){
			return 1;
		} else{
			return 0;
		}
	}
}





