/*
 * registerAddress.c
 *
 *  Created on: Apr 15, 2025
 *      Author: dobao
 */
#include <stdio.h>
#include <stdint.h>
#include "registerAddress.h"


/*
 * Function to offset the Vector Table to RAM (read and write)
 * This help us to modify interrupt handler
 * A useful step for bootloader
 */
void offsetVectorTable(){
	uint32_t* originalVectorTable = (uint32_t*) *VECTORTABLE_OFFSET_REG;
	uint32_t* newVectorTable = (uint32_t*) SRAM_ADDR_DEFAULT; //SRAM default address

	for(int i = 0; i < 0x198; i++){
		*(newVectorTable + i) = *(originalVectorTable + i); //copy the vt to the new offset table
	}

	//Assign VTOR to the new address
	*VECTORTABLE_OFFSET_REG = newVectorTable;
}

