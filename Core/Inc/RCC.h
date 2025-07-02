/*
 * RCC.h
 *
 *  Created on: Jun 30, 2025
 *      Author: dobao
 */

#ifndef INC_RCC_H_
#define INC_RCC_H_

#include <stdio.h>
#include <stdbool.h>
#include <timer.h>
#include "stm32f4xx.h"
#include "registerAddress.h"


#define HSERDY_TIMEOUT	0x2000U
#define PLLRDY_TIMEOUT	0x4000U
#define	SWS_TIMEOUT		0x4000U

#define GET_RCC_REG(mode) (&(RCC_REG -> mode))

/*
 * List of RCC Registers
 */
typedef enum{
	RCC_CR,
	RCC_PLL_CFGR,
	RCC_CFGR,
	RCC_CIR,

	RCC_AHB1_RSTR,
	RCC_AHB2_RSTR,

	RCC_APB1_RSTR,
	RCC_APB2_RSTR,

	RCC_AHB1_ENR,
	RCC_AHB2_ENR,

	RCC_APB1_ENR,
	RCC_APB2_ENR,

	RCC_AHB1_LP_ENR,
	RCC_AHB2_LP_ENR,

	RCC_APB1_LP_ENR,
	RCC_APB2_LP_ENR,

	RCC_BDCR,
	RCC_CSR,

	RCC_SSCGR,
	RCC_PLL_I2S_CFGR,

	RCC_DCK_CFGR,

	RCC_REG_COUNT	//A parameter to store the total number of registers
}RCC_Name_t;



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
 * LIST OF FUNCTION DECLARATIONS
 */
void RCC_init(void);
void writeRCC(uint8_t bitPosition, RCC_Name_t mode, uint32_t value);
uint32_t readRCC(uint8_t bitPosition, RCC_Name_t mode);
void writeBits(volatile uint32_t* reg, uint8_t bitPosition, uint8_t bitWidth, uint32_t value);

#endif /* INC_RCC_H_ */
