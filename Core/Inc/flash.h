/*
 * flash.h
 *
 *  Created on: Jun 1, 2025
 *      Author: dobao
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include <stdio.h>
#include <stdbool.h>

#include "registerAddress.h"

#define GET_FLASH_REG(mode) (&(FLASH_REG -> mode))

typedef enum{
	FLASH_ACR,
	FLASH_KEYR,
	FLASH_OPTKEYR,
	FLASH_SR,
	FLASH_CR,
	FLASH_OPTCR,

	FLASH_REG_COUNT
}Flash_Name_t;

static const uint32_t FLASH_

/*
 * List of function declarations
 */
void writeFlash(uint8_t bitPosition, Flash_Name_t mode, uint32_t value);


#endif /* INC_FLASH_H_ */
