/*
 * i2c.c
 *
 *  Created on: Jul 4, 2025
 *      Author: dobao
 */
#include "i2c.h"

/*
 * ---------------------------------------------------
 * Private Helpers
 * ---------------------------------------------------
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

static volatile uint32_t* I2C1RegLookupTable[I2C_REG_COUNT] = {
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

static volatile uint32_t* I2C2RegLookupTable[I2C_REG_COUNT] = {
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

static volatile uint32_t* I2C3RegLookupTable[I2C_REG_COUNT] = {
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
 * @return	true	Inserted bit indice is valid
 * 			false	...invalid or reserved bit
 */
static inline bool isValidI2CBit(uint8_t bitPosition, I2C_Mode_t mode){
	return ((mode < I2C_REG_COUNT && bitPosition < 32) && ((I2C_VALID_BITS[mode] >> bitPosition) & 0x01));
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
	if(bitPosition > 31) return;
	if(value >= (1U << bitWidth)) return;
	if((bitWidth + bitPosition) > 32) return;

	//Mask off the old bit and OR with the new value
	uint32_t mask = ((1U << bitWidth) - 1) << bitPosition;
	uint32_t shiftedValue = (value << bitPosition) & mask;
	*reg = (*reg & ~mask) | shiftedValue;
}


/*
 * @brief	Read a field of "bitWidth" bits from a register starting at 'bitPosition'
 *
 * @param	reg (pointer) to the register
 * @param	bitPosition		Starting bit position (0-31)
 * @param	bitWidth		Number of bits/bit size that fit @p value
 */
static uint32_t readI2CBits(volatile uint32_t* reg, uint8_t bitPosition, uint8_t bitWidth){
	if(bitWidth == 32){
		return (*reg >> bitPosition); //Full-word: no mask needed
	}
	uint32_t mask = ((1U << bitWidth) - 1U);
	return (*reg >> bitPosition) & mask;
}


void writeI2C(uint8_t bitPosition, I2C_Name_t userI2C, I2C_Mode_t mode, uint32_t value){
	if(!isValidI2CBit(bitPosition, mode)) return;
	if(I2C1RegLookupTable[mode] == NULL) return;
	if(I2C2RegLookupTable[mode] == NULL) return;
	if(I2C3RegLookupTable[mode] == NULL) return;

	volatile uint32_t* reg;

	switch(userI2C){
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

	uint8_t bitWidth = 1;

	switch(mode){
		case I2C_CR1:
			writeI2CBits(reg, bitPosition, bitWidth, value);
			break;

		case I2C_CR2:
			if(bitPosition == 0) bitWidth = 5;
			writeI2CBits(reg, bitPosition, bitWidth, value);
			break;

		case I2C_OAR1:
			if(bitPosition == 1) bitWidth = 7;
			else if(bitPosition == 8) bitWidth = 2;
			writeI2CBits(reg, bitPosition, bitWidth, value);
			break;

		case I2C_OAR2:
			if(bitPosition == 1) bitWidth = 7;
			writeI2CBits(reg, bitPosition, bitWidth, value);
			break;

		case I2C_DR:
			bitWidth = 8;
			writeI2CBits(reg, bitPosition, bitWidth, value);
			break;

		case I2C_SR1:
			writeI2CBits(reg, bitPosition, bitWidth, value);
			break;

		case I2C_SR2:
			if(bitPosition == 8) bitWidth = 8;
			writeI2CBits(reg, bitPosition, bitWidth, value);
			break;

		case I2C_CCR:
			if(bitPosition == 0) bitWidth = 12;
			writeI2CBits(reg, bitPosition, bitWidth, value);
			break;

		case I2C_TRISE:
			bitWidth = 6;
			writeI2CBits(reg, bitPosition, bitWidth, value);
			break;

		case I2C_FLTR:
			if(bitPosition == 0) bitWidth = 4;
			writeI2CBits(reg, bitPosition, bitWidth, value);
			break;

		default: return;
	}
}


uint32_t readI2C(uint8_t bitPosition, I2C_Name_t userI2C, I2C_Mode_t mode){
	const uint32_t ERROR_FLAG = 0xFFFFFFFF;

	if(!isValidI2CBit(bitPosition, mode)) return ERROR_FLAG;
	if(bitPosition > 31) return ERROR_FLAG;
	if(I2C1RegLookupTable[mode] == NULL) return ERROR_FLAG;
	if(I2C2RegLookupTable[mode] == NULL) return ERROR_FLAG;
	if(I2C3RegLookupTable[mode] == NULL) return ERROR_FLAG;

	volatile uint32_t* reg;

	switch(userI2C){
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

	uint8_t bitWidth = 1;
	switch(mode){
		case I2C_CR1:
			return readI2CBits(reg, bitPosition, bitWidth);

		case I2C_CR2:
			if(bitPosition == 0) bitWidth = 6;
			return readI2CBits(reg, bitPosition, bitWidth);

		case I2C_OAR1:
			if(bitPosition == 1) bitWidth = 7;
			else if(bitPosition == 8) bitWidth = 2;
			return readI2CBits(reg, bitPosition, bitWidth);

		case I2C_OAR2:
			if(bitPosition == 1) bitWidth = 7;
			return readI2CBits(reg, bitPosition, bitWidth);

		case I2C_DR:
			bitWidth = 8;
			return readI2CBits(reg, bitPosition, bitWidth);

		case I2C_SR1:
			return readI2CBits(reg, bitPosition, bitWidth);

		case I2C_SR2:
			if(bitPosition == 8) bitWidth = 8;
			return readI2CBits(reg, bitPosition, bitWidth);

		case I2C_CCR:
			if(bitPosition == 0) bitWidth = 12;
			return readI2CBits(reg, bitPosition, bitWidth);

		case I2C_TRISE:
			bitWidth = 6;
			return readI2CBits(reg, bitPosition, bitWidth);

		case I2C_FLTR:
			if(bitPosition == 0) bitWidth = 4;
			return readI2CBits(reg, bitPosition, bitWidth);

		default: return ERROR_FLAG;
	}
}




















