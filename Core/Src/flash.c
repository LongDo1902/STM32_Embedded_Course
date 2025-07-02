/*
 * flash.c
 *
 *  Created on: Jun 1, 2025
 *      Author: dobao
 */


#include "flash.h"

/*
 * @brief	Initialize a lookup table for readFlash() and writeFlash() to access any Flash register generically via an enum index
 * 			instead of hard-coding addresses each time
 *
 * @note	'const keeps the table in Flash'; registers themselve stay 'volatile' because the hardware can change them anytime
 */
static volatile uint32_t* const flashRegLookupTable[FLASH_REG_COUNT] = {
		[FLASH_ACR] 	= GET_FLASH_REG(FLASH_ACR),
		[FLASH_KEYR] 	= GET_FLASH_REG(FLASH_KEYR),
		[FLASH_OPTKEYR]	= GET_FLASH_REG(FLASH_OPTKEYR),
		[FLASH_SR]		= GET_FLASH_REG(FLASH_SR),
		[FLASH_CR]		= GET_FLASH_REG(FLASH_CR),
		[FLASH_OPTCR]	= GET_FLASH_REG(FLASH_OPTCR)
};



void writeFlash(uint8_t bitPosition, Flash_Name_t mode, uint32_t value){
	if(bitPosition > 31) return;

	volatile uint32_t* reg = flashRegLookupTable[mode];
	switch(mode){

	}

}

