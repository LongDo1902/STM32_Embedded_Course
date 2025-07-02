/*
 * flash.c
 *
 *  Created on: Jun 1, 2025
 *      Author: dobao
 */


#include "flash.h"

/*
 * @brief	Initialize a lookup table for readFlash() and writeFlash() to access any Flash register generically via an enum index
 * 			instead osf hard-coding addresses each time
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



/*
 * @brief	A lookup table of **valid-bit masks** for every FLASH register
 * 			The array is indexed by 'FLASH_REG_COUNT'
 */
static const uint32_t FLASH_VALID_BIT[FLASH_REG_COUNT] = {
		[FLASH_ACR] = ~((0xFu << 4) | 0xFFFFE000u), //These bits are reserved
		[FLASH_KEYR] = 0xFFFFFFFFu,
		[FLASH_OPTKEYR] = 0xFFFFFFFFu,
		[FLASH_SR] = ~((0x3u << 2) | (0x7Fu << 9) | 0x7FFFu << 17),
		[FLASH_CR] = ~((1u << 7) | (0x3Fu << 10) | (0x7Fu << 17) | (0x1Fu << 26)),
		[FLASH_OPTCR] = ~((1u << 4) | (0x7F << 24)),
};



/*
 *
 */
void writeFlash(uint8_t bitPosition, Flash_Name_t mode, uint32_t value){
	if(bitPosition > 31) return;

	volatile uint32_t* reg = flashRegLookupTable[mode];
	switch(mode){

	}

}

