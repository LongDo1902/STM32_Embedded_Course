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
#include "timer.h"
#include "stm32f4xx.h"
#include "registerAddress.h"
#include "flash.h"


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



/*
 * LIST OF FUNCTION DECLARATIONS
 */
void RCC_init(void);
void writeRCC(uint8_t bitPosition, RCC_Name_t mode, uint32_t value);
uint32_t readRCC(uint8_t bitPosition, RCC_Name_t mode);
void writeBits(volatile uint32_t* reg, uint8_t bitPosition, uint8_t bitWidth, uint32_t value);
//Hello
#endif /* INC_RCC_H_ */
