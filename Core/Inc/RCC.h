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
#include "registerAddress.h"

/*
 * List of RCC Registers
 */
typedef enum{
	RCC_RC,
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

/*
 * Function Declaration
 */

/*
 * @brief	This function checks if bit position is reserved
 *
 * @param	bitPosition		bit location in a specific desired RCC register
 * @param	mode			RCC's register
 *
 * @return	true: there is not reserved bit selected
 * @return	false: vice versa
 */
static inline bool isValidRccBit(uint8_t bitPosition, RCC_Name_t mode){
	switch(mode){
		case RCC_RC:
			return (bitPosition != 2 &&
					!(bitPosition >= 20 && bitPosition <= 23) &&
					(bitPosition < 28));

		case RCC_PLL_CFGR:
			return (bitPosition != 15 &&
					!(bitPosition >= 18 && bitPosition <= 21) &&
					bitPosition != 23 &&
					bitPosition < 28);

		case RCC_CFGR:
			return (bitPosition != 8 && bitPosition != 9);

		case RCC_CIR:
			return (bitPosition != 6 &&
					bitPosition != 14 &&
					bitPosition != 15 &&
					bitPosition != 22 &&
					bitPosition < 24);

		case RCC_AHB1_RSTR:
			return (bitPosition != 5 &&
					bitPosition != 6 &&
					!(bitPosition >= 8 && bitPosition <= 11) &&
					!(bitPosition >= 13 && bitPosition <= 20) &&
					bitPosition < 23);

		case RCC_AHB2_RSTR:
			return bitPosition == 7;

		case RCC_APB1_RSTR:
			return (!(bitPosition >= 4 && bitPosition <= 10) &&
					bitPosition != 12 &&
					bitPosition != 13 &&
					bitPosition != 16 &&
					!(bitPosition >= 18 && bitPosition <= 20) &&
					!(bitPosition >= 24 && bitPosition <= 27) &&
					bitPosition < 29);

		case RCC_APB2_RSTR:
			return (!(bitPosition >= 1 && bitPosition <= 3) &&
					bitPosition != 6 &&
					bitPosition != 7 &&
					bitPosition != 9 &&
					bitPosition != 10 &&
					bitPosition != 15 &&
					bitPosition != 19 &&
					bitPosition < 21);

		case RCC_AHB1_ENR:
			return ((bitPosition >= 0 && bitPosition <= 4) ||
					bitPosition == 7 ||
					bitPosition == 12 ||
					bitPosition == 21 ||
					bitPosition == 22);

		case RCC_AHB2_ENR:
			return bitPosition == 7;

		case RCC_APB1_ENR:
			return (!(bitPosition >= 4 && bitPosition <= 10) &&
					bitPosition != 13 &&
					bitPosition != 12 &&
					!(bitPosition >= 18 && bitPosition <= 20) &&
					bitPosition != 16 &&
					!(bitPosition >= 24 && bitPosition <= 27) &&
					bitPosition < 29);

		case RCC_APB2_ENR:
			return (!(bitPosition >= 1 && bitPosition <= 3) &&
					bitPosition != 6 &&
					bitPosition != 7 &&
					bitPosition != 9 &&
					bitPosition != 10 &&
					bitPosition != 15 &&
					bitPosition != 19 &&
					bitPosition < 21);

		case RCC_AHB1_LP_ENR:
			return (bitPosition != 5 &&
					bitPosition != 6 &&
					!(bitPosition >= 8 && bitPosition <= 11) &&
					bitPosition != 13 &&
					bitPosition != 14 &&
					!(bitPosition >= 17 && bitPosition <= 20) &&
					bitPosition < 23);

		case RCC_AHB2_LP_ENR:
			return bitPosition == 7;

		case RCC_APB1_LP_ENR:
			return (!(bitPosition >= 4 && bitPosition <= 10) &&
					bitPosition != 12 &&
					bitPosition != 13 &&
					bitPosition != 16 &&
					!(bitPosition >= 18 && bitPosition <= 20) &&
					!(bitPosition >= 24 && bitPosition <= 27) &&
					bitPosition < 29);

		case RCC_APB2_LP_ENR:
			return (!(bitPosition >= 1 && bitPosition <= 3) &&
					bitPosition != 6 &&
					bitPosition != 7 &&
					bitPosition != 9 &&
					bitPosition != 10 &&
					bitPosition != 15 &&
					bitPosition != 19 &&
					bitPosition < 21);

		case RCC_BDCR:
			return (bitPosition < 17 &&
					!(bitPosition >= 10 && bitPosition <= 14) &&
					!(bitPosition >= 4 && bitPosition <= 7));

		case RCC_CSR:
			return (!(bitPosition >= 2 && bitPosition <= 23));

		case RCC_SSCGR:
			return (bitPosition != 28 && bitPosition != 29);

		case RCC_PLL_I2S_CFGR:
			return (!(bitPosition >= 15 && bitPosition <= 27) &&
					bitPosition != 31);

		case RCC_DCK_CFGR:
			return bitPosition == 24;

		default: return false;
	}
}







#endif /* INC_RCC_H_ */
