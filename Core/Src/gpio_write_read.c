/*
 * gpio_write_read.c
 *
 *  Created on: Apr 13, 2025
 *      Author: dobao
 */

#include "gpio_write_read.h"

/*
 * Helper function to flexible enable GPIO clock
 */
void Enable_GPIO_Clock(GPIO_PortName_t port){
	switch(port){
		case my_GPIOA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
		case my_GPIOB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
		case my_GPIOC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
		case my_GPIOD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
		case my_GPIOE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
		case my_GPIOH: __HAL_RCC_GPIOH_CLK_ENABLE(); break;
		default: return;
	}
}



/*
 * Helper function to write bit to a pin to a GPIO for general purposes
 * @param 	port	my_GPIOA if it is port A
 * @param 	pinNum	pin number write my_GPIO_PIN_1 if pin is 1
 * @param 	state	my_GPIO_PIN_SET if the pin is high
 *
 * @Info: see gpio_write_read.h
 */
void writePin(GPIO_Pin_t pinNum, GPIO_PortName_t port, GPIO_Mode_t mode, GPIO_State_t state){
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

		case ODR:
			reg = &GPIOx -> ODR;
			bitWidth = 1;
			bitShift = pinNum * bitWidth;
			break;

		case AFRL:
			if(pinNum > 7) return; //Invalid pin
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
 * @brief Locks the configuration of a specific GPIO pin.
 *        Once locked, the pin's mode, output type, speed, pull-up/down,
 *        and alternate function cannot be changed until the next MCU reset.
 *
 * @param 	pinNum 	The GPIO pin number (0 to 15).
 * @param 	port 	The GPIO port name (e.g., my_GPIOA, my_GPIOB).
 * @return 	true if the lock was successful, false otherwise.
 */

bool GPIO_LockPin(GPIO_Pin_t pinNum, GPIO_PortName_t port){
	if(pinNum > 15) return false;

	GPIO_Register_Offset_t* GPIOx;

	switch(port){
		case my_GPIOA: GPIOx = GPIOA_REG; break; //Assign pointer GPIOx to pointer GPIOA_REG
		case my_GPIOB: GPIOx = GPIOB_REG; break;
		case my_GPIOC: GPIOx = GPIOC_REG; break;
		case my_GPIOD: GPIOx = GPIOD_REG; break;
		case my_GPIOE: GPIOx = GPIOE_REG; break;
		case my_GPIOH: GPIOx = GPIOH_REG; break;
		default: return false; //Invalid port!
	}

	uint16_t pinMask = ((1U << pinNum) & 0xFFFF);

	/*
	 * Lock key write sequence
	 */
	GPIOx -> LCKR = (1U << 16) | pinMask; //Write 1 to bit 16 and | with pinMask
	GPIOx -> LCKR = (0 << 16) | pinMask; //Write 0 to bit 16...
	GPIOx -> LCKR = (1U << 16) | pinMask; //Write 1 to bit 16...
	(void)GPIOx -> LCKR; //Read LCKR

	return (GPIOx -> LCKR & (1U << 16)) != 0;
}



/*
 * @brief	Writes a value to a specific bit field within a selected FLASH register
 * 			Automatically calculates the bitWidth based on the given value.
 * 			Safely handles reserved bitPositions and prevents overflow
 *
 * 	@param	bitPosition		Bit position to begin writting from (0 to 31)
 * 	@param 	mode			Enum indicating which FLASH register to write to
 * 	@param	value			Data to be written; bit width is inferred automatically
 */
void writeFlash(uint8_t bitPosition, Flash_IntF_Mode_t mode, uint32_t value){
	//Get the case address of the FLASH interface register map
	Flash_IntF_Register_Offset_t* flashReg = FLASH_REG;

	//Declare a pointer to the target offset register
	volatile uint32_t* reg;

	//Select the appropriate FLASH offset register based on mode
	switch(mode){
		case FLASH_ACR:
			if((bitPosition >= 4 && bitPosition <= 7) || bitPosition >= 13){ //Reserved bitPosition
				return;
			}
			reg = &flashReg -> FLASH_ACR;
			break;

		case FLASH_KEYR:
			reg = &flashReg -> FLASH_KEYR;
			break;

		case FLASH_OPTKEYR:
			reg = &flashReg -> FLASH_OPTKEYR;
			break;

		case FLASH_SR:
			if(bitPosition == 2 || bitPosition == 3
				|| (bitPosition >= 9 && bitPosition <= 15)
				|| (bitPosition >= 17 && bitPosition <= 31)){ //Reserved bitPosition
				return;
			}
			reg = &flashReg -> FLASH_SR;
			break;

		case FLASH_CR:
			if(bitPosition == 7
				|| (bitPosition >= 10 && bitPosition <= 15)
				|| (bitPosition >= 17 && bitPosition <= 23)
				|| (bitPosition >= 26 && bitPosition <= 30)){ //Reserved bitPosition
				return;
			}
			reg = &flashReg -> FLASH_CR;
			break;

		case FLASH_OPTCR:
			if (bitPosition == 4 || (bitPosition >= 24 && bitPosition <= 30)){
				return;
			}
			reg = &flashReg -> FLASH_OPTCR;
			break;
		default: return; //Invalid mode! Exit early
	}

	//Auto detect bitwidth based on the "value" length
	uint8_t bitWidth = 0;
	uint32_t temp = value;
	while(temp > 0){
		bitWidth++; //Increment the bidWidth by one when ever temp > 0
		temp = temp >> 1; //Shift the temp to the right by 1
	}

	//Prevent overflow
	if (bitPosition + bitWidth > 32){
		return;
	}

	//Mask off the old bit and OR with new value
	uint32_t mask = ((1U << bitWidth) - 1U) << bitPosition;
	uint32_t shiftedValue = (value << bitPosition) & mask;
	*reg = (*reg & ~mask) | shiftedValue;
}



/*
 *	@brief	Read a specific bit from FLASH register safely
 *
 *	@param	bitPosition		Bit position to begin writting from (0 to 31)
 *	@param	mode			the FLASH offset register
 *
 *	@retval		1 if the bit is set
 *				0 if the bit if cleared
 *				-1 if the bit position is reserved or invalid for selected FLASH register
 */
char readFLASH(uint8_t bitPosition, Flash_IntF_Mode_t mode){
	//Get the case address of the FLASH interface register map
	Flash_IntF_Register_Offset_t* flashReg = FLASH_REG;

	//Declare a pointer to the target offset register
	volatile uint32_t* reg;

	//Select the appropriate FLASH offset register based on mode
	switch(mode){
		case FLASH_ACR: //RW
			if((bitPosition >= 4 && bitPosition <= 7) || bitPosition >= 13){ //Reserved bitPosition
				return -1;
			}
			reg = &flashReg -> FLASH_ACR;
			break;

		case FLASH_SR: //RW
			if(bitPosition == 2 || bitPosition == 3
				|| (bitPosition >= 9 && bitPosition <= 15)
				|| (bitPosition >= 17 && bitPosition <= 31)){ //Reserved bitPosition
				return -1;
			}
			reg = &flashReg -> FLASH_SR;
			break;

		case FLASH_CR: //RW
			if(bitPosition == 7
				|| (bitPosition >= 10 && bitPosition <= 15)
				|| (bitPosition >= 17 && bitPosition <= 23)
				|| (bitPosition >= 26 && bitPosition <= 30)){ //Reserved bitPosition
				return -1;
			}
			reg = &flashReg -> FLASH_CR;
			break;

		case FLASH_OPTCR:  //RW
			if (bitPosition == 4 || (bitPosition >= 24 && bitPosition <= 30)){
				return -1;
			}
			reg = &flashReg -> FLASH_OPTCR;
			break;
		default: return -1; //Invalid mode! Exit early
	}

	//Check if there is invalid bit position
	if (bitPosition > 31) return -1;

	uint32_t value = *reg; //Access to the memory space/content in reg pointer
	uint8_t bit = (value >> bitPosition) & 0x1;
	return bit;
}
