/*
 * flash.c
 *
 *  Created on: Jun 1, 2025
 *      Author: dobao
 */
#include "registerAddress.h"
#include "flash.h"
#include "gpio_write_read.h"
#include "stdio.h"

/*
 * @brief	A function to erase a specific sector
 * @param	sectorNum: write 0 to 7 which 0 for sector 0 (becareful when eraing sector 0)
 */
void Flash_Sector_Erase(int sectorNum){
	while (readFLASH(16, FLASH_SR) & 0x1 == 1); //Stay in this loop when there is on going Flash mem operation
	GPIO_WriteFlash(1, FLASH_CR, 1); //Sector erase activate
	GPIO_WriteFlash(3, FLASH_CR, sectorNum); //

}




