/*
 * RCC.c
 *
 *  Created on: Jun 30, 2025
 *      Author: dobao
 */

#include "rcc.h"

void my_RCC_GPIOA_CLK_ENABLE(){
	writeRCC(0, RCC_AHB1_ENR, SET);
}

void my_RCC_GPIOA_CLK_DISABLE(){
	writeRCC(0, RCC_AHB1_ENR, RESET);
}

void my_RCC_GPIOB_CLK_ENABLE(){
	writeRCC(1, RCC_AHB1_ENR, SET);
}

void my_RCC_GPIOB_CLK_DISABLE(){
	writeRCC(1, RCC_AHB1_ENR, RESET);
}

void my_RCC_GPIOC_CLK_ENABLE(){
	writeRCC(2, RCC_AHB1_ENR, SET);
}

void my_RCC_GPIOC_CLK_DISABLE(){
	writeRCC(2, RCC_AHB1_ENR, RESET);
}

void my_RCC_GPIOD_CLK_ENABLE(){
	writeRCC(3, RCC_AHB1_ENR, SET);
}

void my_RCC_GPIOD_CLK_DISABLE(){
	writeRCC(3, RCC_AHB1_ENR, RESET);
}

void my_RCC_GPIOE_CLK_ENABLE(){
	writeRCC(4, RCC_AHB1_ENR, SET);
}

void my_RCC_GPIOE_CLK_DISABLE(){
	writeRCC(4, RCC_AHB1_ENR, RESET);
}

void my_RCC_TIM1_CLK_ENABLE(){

}

void my_RCC_TIM1_CLK_DISABLE(){

}

void my_RCC_SPI1_CLK_ENABLE(){

}


void my_RCC_SPI1_CLK_DISABLE(){

}
/*
 * @brief	Lets readRCC() and writeRCC() access any RCC register generically via an enum index
 * 			instead of hard-coding addresses each time
 *
 * @note	'const keeps the table in Flash; registers themselve stay 'volatile' because the hardware
 * 			can change them.
 */
static volatile uint32_t* const rccRegLookupTable[RCC_REG_COUNT] = {
		[RCC_CR] 		= GET_RCC_REG(RCC_CR),
		[RCC_PLL_CFGR] 	= GET_RCC_REG(RCC_PLL_CFGR),
		[RCC_CFGR] 		= GET_RCC_REG(RCC_CFGR),
		[RCC_CIR] 		= GET_RCC_REG(RCC_CIR),

		[RCC_AHB1_RSTR] = GET_RCC_REG(RCC_AHB1_RSTR),
		[RCC_AHB2_RSTR] = GET_RCC_REG(RCC_AHB2_RSTR),

		[RCC_APB1_RSTR] = GET_RCC_REG(RCC_APB1_RSTR),
		[RCC_APB2_RSTR] = GET_RCC_REG(RCC_APB2_RSTR),

		[RCC_AHB1_ENR]	= GET_RCC_REG(RCC_AHB1_ENR),
		[RCC_AHB2_ENR]	= GET_RCC_REG(RCC_AHB2_ENR),

		[RCC_APB1_ENR]	= GET_RCC_REG(RCC_APB1_ENR),
		[RCC_APB2_ENR] 	= GET_RCC_REG(RCC_APB2_ENR),

		[RCC_AHB1_LP_ENR]	= GET_RCC_REG(RCC_AHB1_LP_ENR),
		[RCC_AHB2_LP_ENR]	= GET_RCC_REG(RCC_AHB2_LP_ENR),

		[RCC_APB1_LP_ENR]	= GET_RCC_REG(RCC_APB1_LP_ENR),
		[RCC_APB2_LP_ENR]	= GET_RCC_REG(RCC_APB2_LP_ENR),

		[RCC_BDCR]			= GET_RCC_REG(RCC_BDCR),
		[RCC_CSR]			= GET_RCC_REG(RCC_CSR),

		[RCC_SSCGR]			= GET_RCC_REG(RCC_SSCGR),
		[RCC_PLL_I2S_CFGR]	= GET_RCC_REG(RCC_PLL_I2S_CFGR),

		[RCC_DCK_CFGR]		= GET_RCC_REG(RCC_DCK_CFGR),
};



/*
 * @brief	A lookup table of **valid-bit masks** for every RCC register
 *			The array is indexed by 'RCC_REG_COUNT'
 */
static const uint32_t RCC_VALID_BITS[RCC_REG_COUNT] = {
		[RCC_CR] = ~((1 << 2) | 0x00F00000 | 0xF0000000),
		[RCC_PLL_CFGR] = ~((1 << 15) | 0x003C0000 | (1 << 23) | 0xF0000000),

		[RCC_CFGR] = ~((1 << 8) | (1 << 9)),
		[RCC_CIR] = ~((1 << 6) | (1 << 14) | (1 << 15) | (1 << 22) | 0xFF000000),

		[RCC_AHB1_RSTR] = ~((1 << 5) | (1 << 6) | 0x00000F00 | 0x001FE000 | 0xFF800000),
		[RCC_AHB2_RSTR] = 1 << 7,

		[RCC_APB1_RSTR] = ~(0x000007F0 | (1 << 12) | (1 << 13) | (1 << 16) | 0x001C0000 | 0x0F000000 | 0xE0000000),
		[RCC_APB2_RSTR] = ~(0x0000000E | (1 << 6) | (1 << 7) | (1 << 9) | (1 << 10) | (1 << 15) | (1 << 19) | 0xFFE00000),

		[RCC_AHB1_ENR] = (0x1F | (1 << 7) | (1 << 12) | (1 << 21) | (1 << 22)),
		[RCC_AHB2_ENR] = 1 << 7,

		[RCC_APB1_ENR] = ~(0x000007F0 | (1 << 12) | (1 << 13) | (1 << 16) | 0x001C0000 | 0x0F000000 | 0xE0000000),
		[RCC_APB2_ENR] = ~(0x0000000E | (1 << 6) | (1 << 7) | (1 << 9) | (1 << 10) | (1 << 15) | (1 << 19) | 0xFFE00000),

		[RCC_AHB1_LP_ENR] = ~((1 << 5) | (1 << 6) | 0x00000F00 | (1 << 13) | (1 << 14) | 0x001E0000 | 0xFF800000),
		[RCC_AHB2_LP_ENR] = 1 << 7,

		[RCC_APB1_LP_ENR] = ~(0x000007F0 | (1 << 12) | (1 << 13) | (1 << 16) | 0x001C0000 | 0x0F000000 | 0xE0000000),
		[RCC_APB2_LP_ENR] = ~(0x0000000E | 0xFFE00000 | (1 << 6) | (1 << 7) | (1 << 9) | (1 << 10) | (1 << 15) | (1 << 19)),

		[RCC_BDCR] = ~(0xFFFE0000 | 0x00007C00 | 0x000000F0),
		[RCC_CSR] = ~(0x00FFFFFC),

		[RCC_SSCGR] = ~((1 << 28) | (1 << 29)),
		[RCC_PLL_I2S_CFGR] = ~((1 << 31) | 0x0FFF8000),
		[RCC_DCK_CFGR] = 1 << 24,
};



/*
 * @brief	Check if a specific bit position in an RCC reg is valid (not reserved)
 *
 * @param	bitPosition		Bit index (0-31) to check within the given RCC reg
 * @param	mode			Enum value representing the RCC register being access
 *
 * @retval	true	bit is valid
 * @retval	false	bit is reserved or inputs are out of range
 */
static inline bool isValidRccBit(uint8_t bitPosition, RCC_Name_t mode){
	return ((mode < RCC_REG_COUNT && bitPosition < 32) && ((RCC_VALID_BITS[mode] >> bitPosition) & 0x01));
}


/*
 * @brief	Initialize the Reset-and-Clock-Control (RCC) unit for STM32F411
 * 			Uses an 8MHz crystal (HSE)
 * 			Configures the main PLL to generate a 100MHz SYSCLK
 * 				VCO_input = 8MHz / 8 = 1MHz
 * 				VCO_output = 1MHz x 200 = 200MHz
 * 				SYSCLK = 200MHz / 2 = 100MHz
 *			Contains a simple timeout loops so the MCU will not hang forever
 */
void RCC_init(void){
	writeRCC(24, RCC_CR, RESET); //Disabe PLL
	writeRCC(16, RCC_CR, SET); //Enable HSE clock

	for(uint32_t t = 0; (readRCC(17, RCC_CR) & 0x01) == 0; ){
		if(++t > HSERDY_TIMEOUT) return; //Give up - crystal failed
	}

	/*
	 * Config PLL
	 * 		f_vco_in = f_pll_clk_input x (PLLN / PLLM)
	 * 		f_pll_clk_out = f_vco_in / PLLP
	 * Scale the clock from 8MHz-HSE clock to 100MHz-PLL output clock
	 */
	writeRCC(22, RCC_PLL_CFGR, SET); //Select HSE as PLL's main source clock
	writeRCC(0, RCC_PLL_CFGR, 8); //PLLM = 8
	writeRCC(6, RCC_PLL_CFGR, 200); //PLLN = 200
	writeRCC(16, RCC_PLL_CFGR, 0b00); //PLLP = 2

	/*
	 * The maximum allowed freq of the low-speed APB1 is 50MHz
	 */
	writeRCC(10, RCC_CFGR, 0b100); //AHB clock divided by 2

	/*
	 * Enable PLL
	 * Wait for PLL locked to the correct frequency / PLL is stable
	 */
	writeRCC(24, RCC_CR, SET);
	for(uint32_t t = 0; (readRCC(25, RCC_CR) & 0x01) == 0; ){
		if(++t > PLLRDY_TIMEOUT) return;
	}

	writeFlash(0, FLASH_ACR, 0x03); //3WS (90MHz < HCLK < 100MHz) (2.7V to 3.6V)

	/*
	 * Select PLL as the system clock
	 * Wait for the switch (hardware waiting)
	 */
	writeRCC(0, RCC_CFGR, 0b10); //Select PLL as system clock
	for(uint32_t t = 0; (readRCC(2, RCC_CFGR) & 0b11) != 0b10; ){
		if(++t > SWS_TIMEOUT) return;
	}
}



/*
 * @brief	Reads a bit or multi bits from a specific RCC register
 *
 * This function reads a value starting at the given bit position from the specific RCC register
 * For a certain regs, specific multi-bit fields are automatically detected.
 *
 * For example	(e.g. PLLN[14:6] at bit 6) the function automatically reads the full width and
 * 				returns the value right-aligned.
 *
 * @param	bitPosition		The starting bit position (0-31) to read from
 * @param	mode			The RCC reg to access (from RCC_Name_t enum)
 *
 * @retval	Value read from the bitfield (default bitWidth is 1) or 0xFFFFFFF on error
 *
 * @note	The table 'rccRegLookupTable[]' must match the ::RCC_Name_t enum exactly; otherwise
 * 			the function will read the wrong addr.
 */
uint32_t readRCC(uint8_t bitPosition, RCC_Name_t mode){
	const uint32_t ERROR_FLAG = 0xFFFFFFFF;

	if(bitPosition > 31) return ERROR_FLAG;
	if(!isValidRccBit(bitPosition, mode)) return ERROR_FLAG;

	uint8_t bitWidth = 1; //Default 1-bit field
	volatile uint32_t* reg = rccRegLookupTable[mode];

	switch(mode){
		case RCC_CR:
			/*
			 * HSITRIM[4:0] at bit 3
			 * HSICAL[7:0] at bit 8
			 */
			if(bitPosition == 3) bitWidth = 5; //
			else if(bitPosition == 8) bitWidth = 8;

			return readBits(reg, bitPosition, bitWidth);


		case RCC_PLL_CFGR:
			/*
			 * PLLMx[5:0] at bit 0
			 * PLLPx[1:0] at bit 16
			 * PLLQx[3:0] at bit 24
			 */

			if(bitPosition == 0) bitWidth = 6;
			else if(bitPosition == 6) bitWidth = 9;
			else if(bitPosition == 16) bitWidth = 2;
			else if(bitPosition == 24) bitWidth = 4;

			return readBits(reg, bitPosition, bitWidth);


		case RCC_CFGR:
			/*
			 * SW[1:0] at bit 0
			 * SWS[1:0] at bit 2
			 * ...
			 */
			if(bitPosition == 0 || bitPosition == 2 || bitPosition == 21 || bitPosition == 30) bitWidth = 2;
			else if(bitPosition == 4) bitWidth = 4;
			else if(bitPosition == 10 || bitPosition == 13 || bitPosition == 24 || bitPosition == 27) bitWidth = 3;
			else if(bitPosition == 16) bitWidth = 5;

			return readBits(reg, bitPosition, bitWidth);


		case RCC_CIR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_AHB1_RSTR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_AHB2_RSTR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_APB1_RSTR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_APB2_RSTR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_AHB1_ENR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_AHB2_ENR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_APB1_ENR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_APB2_ENR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_AHB1_LP_ENR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_AHB2_LP_ENR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_APB1_LP_ENR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_APB2_LP_ENR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_BDCR:
			if(bitPosition == 8) bitWidth = 2;
			return readBits(reg, bitPosition, bitWidth);


		case RCC_CSR:
			return readBits(reg, bitPosition, bitWidth);


		case RCC_SSCGR:
			if(bitPosition == 0) bitWidth = 13;
			else if(bitPosition == 13) bitWidth = 15;

			return readBits(reg, bitPosition, bitWidth);


		case RCC_PLL_I2S_CFGR:
			if(bitPosition == 0) bitWidth = 6;
			else if(bitPosition == 6) bitWidth = 9;
			else if(bitPosition == 28) bitWidth = 3;

			return readBits(reg, bitPosition, bitWidth);


		case RCC_DCK_CFGR:
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
void writeRCC(uint8_t bitPosition, RCC_Name_t mode, uint32_t value){
	if(!isValidRccBit(bitPosition, mode)) return; //Skip if the target bit is reserved or invalid
	if(rccRegLookupTable[mode] == NULL) return; //Extra safety: make sure the register pointer is not null

	volatile uint32_t* reg = rccRegLookupTable[mode]; //Get pointer to the target RCC reg

	uint8_t bitWidth = 1;

	switch(mode){
		case RCC_CR:
			/*
			 * HSITRIM[4:0] at bit 3
			 * HSICAL[7:0] at bit 8
			 */
			if(bitPosition == 3) bitWidth = 5;
			else if(bitPosition == 8) bitWidth = 8;
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_PLL_CFGR:
			/*
			 * PLLMx[5:0] at bit 0
			 * PLLPx[1:0] at bit 16
			 * PLLQx[3:0] at bit 24
			 */

			if(bitPosition == 0) bitWidth = 6;
			else if(bitPosition == 6) bitWidth = 9;
			else if(bitPosition == 16) bitWidth = 2;
			else if(bitPosition == 24) bitWidth = 4;
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_CFGR:
			/*
			 * SW[1:0] at bit 0
			 * SWS[1:0] at bit 2
			 * ...
			 */
			if(bitPosition == 0 || bitPosition == 2 || bitPosition == 21 || bitPosition == 30) bitWidth = 2;
			else if(bitPosition == 4) bitWidth = 4;
			else if(bitPosition == 10 || bitPosition == 13 || bitPosition == 24 || bitPosition == 27) bitWidth = 3;
			else if(bitPosition == 16) bitWidth = 5;
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_CIR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_AHB1_RSTR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_AHB2_RSTR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_APB1_RSTR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_APB2_RSTR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_AHB1_ENR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_AHB2_ENR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_APB1_ENR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_APB2_ENR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_AHB1_LP_ENR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;

		case RCC_AHB2_LP_ENR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;

		case RCC_APB1_LP_ENR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;

		case RCC_APB2_LP_ENR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;

		case RCC_BDCR:
			if(bitPosition == 8) bitWidth = 2;
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_CSR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_SSCGR:
			if(bitPosition == 0) bitWidth = 13;
			else if(bitPosition == 13) bitWidth = 15;

			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_PLL_I2S_CFGR:
			if(bitPosition == 0) bitWidth = 6;
			else if(bitPosition == 6) bitWidth = 9;
			else if(bitPosition == 28) bitWidth = 3;

			writeBits(reg, bitPosition, bitWidth, value);
			break;


		case RCC_DCK_CFGR:
			writeBits(reg, bitPosition, bitWidth, value);
			break;

		default: return;
	}
}


/*
 * @brief	Write a bit-field into a 32-bit reg
 *
 * @param	bitPosition		Bit location want to write to the specific reg
 * @param	bitWidth		Width of the field in bits (1-31)
 * @param	value			Value of be written (must fit in @p bitWidth)
 *
 * The function leaves the register unchanged if
 * 		@p value is too large for the field
 * 		@p bitWidth is 32 (shifting by 32 is undefined_
 * 		The field would spill past bit 31
 */
void writeBits(volatile uint32_t* reg, uint8_t bitPosition, uint8_t bitWidth, uint32_t value){
	if(value >= (1U << bitWidth)) return;
	if(bitWidth >= 32) return; //Prevent undefined behavior: 32-bit shift is invalid on 32-bit targets
	if((bitWidth + bitPosition) > 32) return; //Prevent writing beyong 32-bit reg boundary

	//Mask off the old bit and or with new bit
	uint32_t mask = ((1U << bitWidth) - 1U) << bitPosition;
	uint32_t shiftedValue = (value << bitPosition) & mask;
	*reg = (*reg & ~mask) | shiftedValue;
}

