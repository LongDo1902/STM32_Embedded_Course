/*
 * RCC.c
 *
 *  Created on: Jun 30, 2025
 *      Author: dobao
 */

#include <rcc.h>


/*
 * @brief	Reads a bit or multi bits from a specific RCC register
 *
 * This function reads a value starting at the given bit position from the specific RCC register
 * For a certain regs, specific multi-bit fields are automatically detected
 *
 * @param	bitPosition		The starting bit position (0-31) to read from
 * @param	mode			The RCC reg to access (from RCC_Name_t enum)
 *
 * @retval	Value read from the bitfield (default bitWidth is 1) or 0xFFFFFFF on error
 */
uint32_t readRcc(uint8_t bitPosition, RCC_Name_t mode){
	const uint32_t ERROR_FLAG = 0xFFFFFFFF;

	if(bitPosition > 31) return ERROR_FLAG;
	if(!isValidRccBit(bitPosition, mode)) return ERROR_FLAG;

	uint8_t bitWidth = 1; //Default 1-bit field
	volatile uint32_t* reg;

	switch(mode){
		case RCC_RC:
			reg = GET_RCC_REG(RCC_RC);
			/*
			 * Multi-bit fields in RCC_RC
			 */
			if(bitPosition == 3) bitWidth = 5;
			else if(bitPosition == 8) bitWidth = 8;

			return readBits(reg, bitPosition, bitWidth);


		case RCC_PLL_CFGR:
			reg = GET_RCC_REG(RCC_PLL_CFGR);

			if(bitPosition == 0) bitWidth = 6;
			else if(bitPosition == 6) bitWidth = 9;
			else if(bitPosition == 16) bitWidth = 2;
			else if(bitPosition == 24) bitWidth = 4;

			return readBits(reg, bitPosition, bitWidth);


		case RCC_CFGR:
			reg = GET_RCC_REG(RCC_CFGR);
			if(bitPosition == 0 || bitPosition == 2 || bitPosition == 21 || bitPosition == 30) bitWidth = 2;
			else if(bitPosition == 4) bitWidth = 4;
			else if(bitPosition == 10 || bitPosition == 13 || bitPosition == 24 || bitPosition == 27) bitWidth = 3;
			else if(bitPosition == 16) bitWidth = 5;

			return readBits(reg, bitPosition, bitWidth);


		case RCC_CIR:
			reg = GET_RCC_REG(RCC_CIR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_AHB1_RSTR:
			reg = GET_RCC_REG(RCC_AHB1_RSTR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_AHB2_RSTR:
			reg = GET_RCC_REG(RCC_AHB2_RSTR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_APB1_RSTR:
			reg = GET_RCC_REG(RCC_APB1_RSTR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_APB2_RSTR:
			reg = GET_RCC_REG(RCC_APB2_RSTR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_AHB1_ENR:
			reg = GET_RCC_REG(RCC_AHB1_ENR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_AHB2_ENR:
			reg = GET_RCC_REG(RCC_AHB2_ENR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_APB1_ENR:
			reg = GET_RCC_REG(RCC_APB1_ENR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_APB2_ENR:
			reg = GET_RCC_REG(RCC_APB2_ENR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_AHB1_LP_ENR:
			reg = GET_RCC_REG(RCC_AHB1_LP_ENR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_AHB2_LP_ENR:
			reg = GET_RCC_REG(RCC_AHB2_LP_ENR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_APB1_LP_ENR:
			reg = GET_RCC_REG(RCC_APB1_LP_ENR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_APB2_LP_ENR:
			reg = GET_RCC_REG(RCC_APB2_LP_ENR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_BDCR:
			reg = GET_RCC_REG(RCC_BDCR);
			if(bitPosition == 8) bitWidth = 2;

			return readBits(reg, bitPosition, bitWidth);


		case RCC_CSR:
			reg = GET_RCC_REG(RCC_CSR);
			return readBits(reg, bitPosition, bitWidth);


		case RCC_SSCGR:
			reg = GET_RCC_REG(RCC_SSCGR);
			if(bitPosition == 0) bitWidth = 13;
			else if(bitPosition == 13) bitWidth = 15;

			return readBits(reg, bitPosition, bitWidth);


		case RCC_PLL_I2S_CFGR:
			reg = GET_RCC_REG(RCC_PLL_I2S_CFGR);
			if(bitPosition == 0) bitWidth = 6;
			else if(bitPosition == 6) bitWidth = 9;
			else if(bitPosition == 28) bitWidth = 3;

			return readBits(reg, bitPosition, bitWidth);


		case RCC_DCK_CFGR:
			reg = GET_RCC_REG(RCC_DCK_CFGR);
			return readBits(reg, bitPosition, bitWidth);


		default: return ERROR_FLAG;
	}
}



/*
 * @brief	Write a value to a specific bitfield within an RCC register
 *
 * This function checks if the bit position and mode are valid, then writes
 * a bitfield to the corresponding RCC reg without affecting other bits.
 * It uses a lookup table to get the RCC register address, and ensures safe bit manipulation
 * even for multi-bit values.
 *
 * @param	bitPosition		The starting bit position (0-31) to write the value
 * @param	mode			The RCC register to modify (from RCC_Name_t enum)
 * @param 	value			The value to write (can span multiple bits)
 */
void writeRcc(uint8_t bitPosition, RCC_Name_t mode, uint32_t value){
	if(!isValidRccBit(bitPosition, mode)) return; //Skip if the target bit is reserved or invalid
	if(rccRegLookupTable[mode] == NULL) return; //Extra safety: make sure the register pointer is not null

	volatile uint32_t* reg = rccRegLookupTable[mode]; //Get pointer to the target RCC reg

	//Calculate how many bits are needed to represent 'value'
	uint32_t bitWidth = 0;
	uint32_t temp = value;

	while(temp > 0){
		bitWidth++;
		temp >>= 1;
	}

	if(value == 0) bitWidth = 1;
	if(bitWidth == 32) return; //Prevent undefined behavior: 32-bit shift is invalid on 32-bit targets
	if((bitWidth + bitPosition) > 32) return; //Prevent writing beyond 32-bit reg boundary

	//Mask off the old bit and or with new bit
	uint32_t mask = ((1U << bitWidth) - 1U) << bitPosition;
	uint32_t shiftedValue = (value << bitPosition) & mask;
	*reg = (*reg & ~mask) | shiftedValue;
}




