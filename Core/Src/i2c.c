/*
 * @file	i2c.c
 *
 *  Created on: Jul 4, 2025
 *      Author: dobao
 */
#include "i2c.h"

/*
 * -----------------------------------------------------------------
 * Private Helpers
 * -----------------------------------------------------------------
 */

/*
 * @brief	Mask of valid (non-reserved) bits for every I2C reg
 * 			A zero marks a reserved bit that must not be written
 *
 * 			Index:  ::I2C_Mode_t
 */
static const uint32_t I2C_VALID_BITS[I2C_REG_COUNT] = {
		[I2C_CR1] = ~((1u << 2) | (1u << 14)),
		[I2C_CR2] = ~((0b11 << 6) | (0b111 << 13)),
		[I2C_OAR1] = ~(0b11111 << 10),
		[I2C_OAR2] = ~(0xFF << 8),
		[I2C_DR] = ~(0xFF << 8),
		[I2C_SR1] = ~((1u << 5) | (1u << 13)),
		[I2C_SR2] = ~(1u << 3),
		[I2C_CCR] = ~(0b11 << 12),
		[I2C_TRISE] = ~(0x3FF << 6),
		[I2C_FLTR] = ~(0x7FF << 5),
};

/*
 * @brief	Lookup table for the I2C1 peripheral register
 *
 * 			This static pointer array maps each value of ::I2C_Mode_t (index) to memory-mapped address
 * 			of the corresponding I2C1 Register. Using the table avoids open-coded 'switch'/'if' blocks
 * 			and makes register access simply
 *
 * 			**volatile** 	Tells the compiler that the pointed-to registers may change asynchronously
 * 							(hardware), so every read/write must be kept.
 *
 * 			**static**		Internal linkage only
 *
 * 			**I2C_REG_COUNT**	Array Length
 */
#define REG_TABLE_ATTR static volatile uint32_t* const

REG_TABLE_ATTR I2C1RegLookupTable[I2C_REG_COUNT] = {
		[I2C_CR1] = GET_I2C1_REG(I2C_CR1),
		[I2C_CR2] = GET_I2C1_REG(I2C_CR2),
		[I2C_OAR1] = GET_I2C1_REG(I2C_OAR1),
		[I2C_OAR2] = GET_I2C1_REG(I2C_OAR2),
		[I2C_DR] = GET_I2C1_REG(I2C_DR),
		[I2C_SR1] = GET_I2C1_REG(I2C_SR1),
		[I2C_SR2] = GET_I2C1_REG(I2C_SR2),
		[I2C_CCR] = GET_I2C1_REG(I2C_CCR),
		[I2C_TRISE] = GET_I2C1_REG(I2C_TRISE),
		[I2C_FLTR] = GET_I2C1_REG(I2C_FLTR),
};

REG_TABLE_ATTR I2C2RegLookupTable[I2C_REG_COUNT] = {
		[I2C_CR1] = GET_I2C2_REG(I2C_CR1),
		[I2C_CR2] = GET_I2C2_REG(I2C_CR2),
		[I2C_OAR1] = GET_I2C2_REG(I2C_OAR1),
		[I2C_OAR2] = GET_I2C2_REG(I2C_OAR2),
		[I2C_DR] = GET_I2C2_REG(I2C_DR),
		[I2C_SR1] = GET_I2C2_REG(I2C_SR1),
		[I2C_SR2] = GET_I2C2_REG(I2C_SR2),
		[I2C_CCR] = GET_I2C2_REG(I2C_CCR),
		[I2C_TRISE] = GET_I2C2_REG(I2C_TRISE),
		[I2C_FLTR] = GET_I2C2_REG(I2C_FLTR),
};

REG_TABLE_ATTR I2C3RegLookupTable[I2C_REG_COUNT] = {
		[I2C_CR1] = GET_I2C3_REG(I2C_CR1),
		[I2C_CR2] = GET_I2C3_REG(I2C_CR2),
		[I2C_OAR1] = GET_I2C3_REG(I2C_OAR1),
		[I2C_OAR2] = GET_I2C3_REG(I2C_OAR2),
		[I2C_DR] = GET_I2C3_REG(I2C_DR),
		[I2C_SR1] = GET_I2C3_REG(I2C_SR1),
		[I2C_SR2] = GET_I2C3_REG(I2C_SR2),
		[I2C_CCR] = GET_I2C3_REG(I2C_CCR),
		[I2C_TRISE] = GET_I2C3_REG(I2C_TRISE),
		[I2C_FLTR] = GET_I2C3_REG(I2C_FLTR),
};

/*
 * @brief	Check that 'mode' is in range and 'bitPosition' is not reserved
 * 			This function flexibly check if multi-bits are valid
 *
 * @return	true	Inserted bit indice is valid
 * 			false	...invalid or reserved bit
 */
static inline bool isValidI2CBit(uint8_t bitPosition, uint8_t bitWidth, I2C_Mode_t mode){
	if(mode >= I2C_REG_COUNT || bitWidth == 0 || ((bitPosition + bitPosition) > 32)) return false;
	uint32_t mask = ((bitWidth == 32) ? 0xFFFFFFFFu : ((1U << bitWidth) - 1U) << bitPosition);

	return (I2C_VALID_BITS[mode] & mask) == mask;
}

/*
 * @brief	Generic masked write helper
 *
 * @param	reg		Pointers to the register
 * @param	bitPosition		First bit of the field
 * @param	bitWidth		Field width in bits
 * @param	value			Field value (must fit in @p bitWidth)
 */
static void writeI2CBits(volatile uint32_t* reg, uint8_t bitPosition, uint8_t bitWidth, uint32_t value){
	/*
	 * The function leaves the register unchanged if
	 * 		@p bitPosition larger than 31 because shifting by 32 is undefined in C
	 * 		@p value is too large for the field
	 * 		The field would spill past bit 31
	 */
	if(bitPosition > 31 || bitWidth > 32) return;
	if(bitWidth < 32 && value >= (1U << bitWidth)) return;
	if((bitWidth + bitPosition) > 32) return;

	//Mask off the old bit and OR with the new value
	uint32_t mask = (bitWidth == 32) ? 0xFFFFFFFFu : ((1U << bitWidth) - 1) << bitPosition;
	uint32_t shiftedValue = (value << bitPosition);
	*reg = (*reg & ~mask) | (shiftedValue & mask);
}


/*
 * @brief	Read a field of "bitWidth" bits from a register starting at 'bitPosition'
 *
 * @param	reg (pointer) to the register
 * @param	bitPosition		Starting bit position (0-31)
 * @param	bitWidth		Number of bits/bit size that fit @p value
 */
static uint32_t readI2CBits(volatile uint32_t* reg, uint8_t bitPosition, uint8_t bitWidth){
	if(bitWidth == 32) return *reg; //Full-word: no mask needed

	uint32_t mask = ((1U << bitWidth) - 1U);
	return (*reg >> bitPosition) & mask;
}


/*
 * @brief	Enable GPIOs' clock
 */
static void enableGPIOClock(GPIO_PortName_t port){
	switch(port){
		case my_GPIOA: my_RCC_GPIOA_CLK_ENABLE(); break;
		case my_GPIOB: my_RCC_GPIOB_CLK_ENABLE(); break;
		case my_GPIOC: my_RCC_GPIOC_CLK_ENABLE(); break;
		case my_GPIOD: my_RCC_GPIOD_CLK_ENABLE(); break;
		case my_GPIOE: my_RCC_GPIOE_CLK_ENABLE(); break;
		case my_GPIOH: my_RCC_GPIOH_CLK_ENABLE(); break;
		default: return;
	}
}


/*
 * @brief	A helper function to intialize SCL pin for the selected I2C peripheral
 *
 * The function performs:
 * 		1. Validation of the <pin, port> tuple against the requested I2C bus
 * 		2. GPIO setup in Alternate Function Open-Drain mode, very-high speed.
 * 		3. Proper pull-up handling:
 * 			+ PB6 on STM32F411 Discovery Board already has an external pull-up resistor
 * 			+ All other SCL pins enable the internal pull-up.
 *
 * @param	sclPin		GPIO pin constant
 * @param	sclPort		GPIO port constant
 * @param	i2cBus		I2C peripheral (my_I2C1, my_I2C2, my_I2C3)
 */

static I2C_Status_t I2C_sclPinInit(GPIO_Pin_t sclPin,
								   GPIO_PortName_t sclPort,
								   I2C_Name_t i2cBus){
	/* Validate pin, port, and bus first */
	switch(i2cBus){
		case my_I2C1:
			if(!(sclPort == my_GPIOB && (sclPin == my_GPIO_PIN_6 || sclPin == my_GPIO_PIN_8))){
				return I2C_INVALID_PIN;
			}
			break;

		case my_I2C2:
			if(!(sclPort == my_GPIOB && sclPin == my_GPIO_PIN_10)){
				return I2C_INVALID_PIN;
			}
			break;

		case my_I2C3:
			if(!(sclPort == my_GPIOA && sclPin == my_GPIO_PIN_8)){
				return I2C_INVALID_PIN;
			}
			break;

		default: return I2C_INVALID_BUS;
	}

	enableGPIOClock(sclPort);
	const GPIO_Mode_t afrReg = (sclPin < my_GPIO_PIN_8) ? AFRL : AFRH;
	const bool hasExtPullUp = (sclPin == my_GPIO_PIN_6 && sclPort == my_GPIOB);

	/* Config GPIO for I2C purpose */
	writePin(sclPin, sclPort, MODER, 0b10); //Set pin to Alternate Function Mode
	writePin(sclPin, sclPort, OTYPER, SET); //Set pin to Open-Drain mode
	writePin(sclPin, sclPort, OSPEEDR, 0b11); //Set a very-high speed output pin
	writePin(sclPin, sclPort, PUPDR, hasExtPullUp ? 0b00 : 0b01); //0b00 = floating, 0b01 = pull-up
	writePin(sclPin, sclPort, afrReg, AF4);

	return I2C_OK;
}


static I2C_Status_t I2C_sdaPinInit(GPIO_Pin_t sdaPin, GPIO_PortName_t sdaPort, I2C_Name_t i2cBus){
	GPIO_State_t alternateFuncSel = 0;

	switch(i2cBus){
		case my_I2C1:
			if(sdaPort == my_GPIOB && sdaPin == my_GPIO_PIN_7){
				alternateFuncSel = AF4;
			} else return I2C_INVALID_PIN;
			break;

		case my_I2C2:
			if(sdaPort == my_GPIOB && sdaPin == my_GPIO_PIN_11){
				alternateFuncSel = AF4;
			}
			else if(sdaPort == my_GPIOB && sdaPin == my_GPIO_PIN_3){
				alternateFuncSel = AF9;
			}
			else return I2C_INVALID_PIN;
			break;

		case my_I2C3:
			if(sdaPort == my_GPIOC && sdaPin == my_GPIO_PIN_9){
				alternateFuncSel = AF4;
			}
			else if((sdaPin == my_GPIO_PIN_4 && sdaPort == my_GPIOB) ||
					(sdaPin == my_GPIO_PIN_8 && sdaPort == my_GPIOB)){
				alternateFuncSel = AF9;
			}
			else return I2C_INVALID_PIN;
			break;

		default: return I2C_INVALID_BUS;
	}

	enableGPIOClock(sdaPort);
	const GPIO_Mode_t afrReg = (sdaPin < my_GPIO_PIN_8) ? AFRL : AFRH;
	const bool hasExtPullUp = (sdaPort == my_GPIOB && sdaPin == my_GPIO_PIN_9);

	/* Config GPIO for I2C purpose*/
	writePin(sdaPin, sdaPort, MODER, 0b10); //Set pin to Alternate Function Mode
	writePin(sdaPin, sdaPort, OTYPER, SET); //Set pin to Open-Drain mode
	writePin(sdaPin, sdaPort, OSPEEDR, 0b11); //Set a very-high speed output pin
	writePin(sdaPin, sdaPort, PUPDR, hasExtPullUp ? 0b00 : 0b01); //0b00 = floating, 0b01 = internal pull-up resistor
	writePin(sdaPin, sdaPort, afrReg, alternateFuncSel);

	return I2C_OK;
}


/*
 * --------------------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------------------
 */

/*
 * @brief	Intialize SCL and SDA pin of selected I2C peripheral
 *
 * @return	I2C_OK if everything is okay
 * 			I2C_INVALID_BUS if user input incorrect I2C peripheral.
 */
I2C_Status_t I2C_pinInit(GPIO_Pin_t sclPin, GPIO_PortName_t sclPort,
				 	 	 GPIO_Pin_t sdaPin, GPIO_PortName_t sdaPort,
						 I2C_Name_t i2cBus){
	switch(i2cBus){
		case my_I2C1: my_RCC_I2C1_CLK_ENABLE(); break;
		case my_I2C2: my_RCC_I2C2_CLK_ENABLE(); break;
		case my_I2C3: my_RCC_I2C3_CLK_ENABLE(); break;
		default: return I2C_INVALID_BUS;
	}
	I2C_sclPinInit(sclPin, sclPort, i2cBus);
	I2C_sdaPinInit(sdaPin, sdaPort, i2cBus);
	return I2C_OK;
}

/*
 * @brief	Write a bit-field to an I2C peripheral register
 *
 * 			Figures out how many bits the setting needs
 * 			Won't touch ay bits the datasheet says are off-limits.
 * 			Changes only the bits you asked for, leaving the rest unchanged.
 * 			Does nothing at all if you pass a bad argument or pick an I2C unit that does not exit
 *
 * @param	bitPosition		The LSB index of the field (0-31)
 * @param	i2cBus			Which peripheral to talk to (my_I2C1, my_I2C2, my_I2C3)
 * @param	mode			Which register of I2C to access (enum @ref I2C_Mode_t)
 *
 * @param	value			The value to write. There is a helper function to check if
 * 							@p value fits into the target field width; if it does not,
 * 							the call ignore
 */
void writeI2C(uint8_t bitPosition, I2C_Name_t i2cBus, I2C_Mode_t mode, uint32_t value){
	//Early sanity
	uint8_t bitWidth = 1;

	switch(mode){
		case I2C_CR2:
			if(bitPosition == 0) bitWidth = 6;
			break;

		case I2C_OAR1:
			if(bitPosition == 1) bitWidth = 7;
			else if(bitPosition == 8) bitWidth = 2;
			break;

		case I2C_OAR2:
			if(bitPosition == 1) bitWidth = 7;
			break;

		case I2C_DR:
			bitWidth = 8;
			break;

		case I2C_SR2:
			if(bitPosition == 8) bitWidth = 8;
			break;

		case I2C_CCR:
			if(bitPosition == 0) bitWidth = 12;
			break;

		case I2C_TRISE:
			bitWidth = 6;
			break;

		case I2C_FLTR:
			if(bitPosition == 0) bitWidth = 0;
			break;

		default: return;
	}

	if(!isValidI2CBit(bitPosition, bitWidth, mode)) return;

	/* Map bus -> Register pointer	 */
	volatile uint32_t* reg = NULL;

	switch(i2cBus){
		case my_I2C1:
			reg = I2C1RegLookupTable[mode];
			break;

		case my_I2C2:
			reg = I2C2RegLookupTable[mode];
			break;

		case my_I2C3:
			reg = I2C3RegLookupTable[mode];
			break;

		default: return;
	}
	if(reg == NULL) return; //Peripheral not present on this part

	//Disallow writes to read-only status reg except write to clear bits
	if(mode == I2C_SR1 || mode == I2C_SR2){
		//Only allow write-to-clear its (set value to 0 to clear)
		if(value != 0){
			return;
		}
	}

	writeI2CBits(reg, bitPosition, bitWidth, value);
}


/*
 * @brief	Read a bit-field from an I2C peripheral register
 *
 * @param	bitPosition		The LSB index of the field (0-31)
 * @param	i2cBus			Which peripheral to talk to (my_I2C1, my_I2C2, my_I2C3)
 * @param	mode			Which register of I2C to access (enum @ref I2C_Mode_t)
 *
 * @param	value			The value to write. There is a helper function to check if
 * 							@p value fits into the target field width; if it does not,
 * 							the call ignore
 *
 * @return	The extracted field value on success.
 * 			If the cal is invalid, the constant @c 0xFFFFFFFF is returned as an ERROR_FLAG
 */
uint32_t readI2C(uint8_t bitPosition, I2C_Name_t i2cBus, I2C_Mode_t mode){
	uint32_t const ERROR_FLAG = 0xFFFFFFFF;
	uint8_t bitWidth = 1;

	switch(mode){
		case I2C_CR2:
			if(bitPosition == 0) bitWidth = 6;
			break;

		case I2C_OAR1:
			if(bitPosition == 1) bitWidth = 7;
			else if(bitPosition == 8) bitWidth = 2;
			break;

		case I2C_OAR2:
			if(bitPosition == 1) bitWidth = 7;
			break;

		case I2C_DR:
			bitWidth = 8;
			break;

		case I2C_SR2:
			if(bitPosition == 8) bitWidth = 8;
			break;

		case I2C_CCR:
			if(bitPosition == 0) bitWidth = 12;
			break;

		case I2C_TRISE:
			bitWidth = 6;
			break;

		case I2C_FLTR:
			if(bitPosition == 0) bitWidth = 0;
			break;

		default: return ERROR_FLAG;
	}

	if(!isValidI2CBit(bitPosition, bitWidth, mode)) return ERROR_FLAG;

	volatile uint32_t* reg = NULL;

	switch(i2cBus){
		case my_I2C1:
			reg = I2C1RegLookupTable[mode];
			break;

		case my_I2C2:
			reg = I2C2RegLookupTable[mode];
			break;

		case my_I2C3:
			reg = I2C3RegLookupTable[mode];
			break;

		default: return ERROR_FLAG;
	}

	if(reg == NULL) return ERROR_FLAG;

	return readI2CBits(reg, bitPosition, bitWidth);
}









