/*
 * exti.c
 *
 *  Created on: Jun 26, 2025
 *      Author: dobao
 */

#include "exti.h"


void NVIC_enableIRQ(uint16_t irqNumber, NVIC_Name_t nvicName){
	if(irqNumber > 85) return;

	uint8_t regIdx = irqNumber / 32;
	uint8_t bitPos = irqNumber % 32;

	switch(nvicName){
		case NVIC_ISER:
			NVIC_REG -> _ISER[regIdx] = (SET << bitPos);
			break;
		case NVIC_ICER:
			NVIC_REG -> _ICER[regIdx] = (SET << bitPos);
			break;

		case NVIC_ISPR:
			NVIC_REG -> _ISPR[regIdx] = (SET << bitPos);
			break;

		case NVIC_ICPR:
			NVIC_REG -> _ICPR[regIdx] = (SET << bitPos);
			break;

		default: return;
	}
}

void NVIC_disableIRQ(uint32_t irqNumber, NVIC_Name_t nvicName){
	if(irqNumber > 85) return;

	uint8_t regIdx = irqNumber / 32;
	uint8_t bitPos = irqNumber % 32;

	switch(nvicName){
		case NVIC_ISER:
			NVIC_REG -> _ISER[regIdx] = (RESET << bitPos);
			break;
		case NVIC_ICER:
			NVIC_REG -> _ICER[regIdx] = (RESET << bitPos);
			break;

		case NVIC_ISPR:
			NVIC_REG -> _ISPR[regIdx] = (RESET << bitPos);
			break;

		case NVIC_ICPR:
			NVIC_REG -> _ICPR[regIdx] = (RESET << bitPos);
			break;

		default: return;
	}
}


void NVIC_writeIPR(uint16_t irqNumber, uint8_t priority){
	if(irqNumber > 85) return;
	NVIC_REG -> _IPR[irqNumber] = priority & 0xF0; //Top 4 bits only are used
}

/*
 * Interrupts for GPIOs
 */
void writeEXTI(uint8_t bitPosition, EXTI_Mode_t mode, GPIO_State_t state){
	volatile uint32_t* reg; //A pointer to store the address and access to the memory space at that address
	switch(mode){
		case IMR:

			break;

		case EMR:

			break;


		case RTSR:

			break;

		case FTSR:

			break;

		case SWIER:

			break;

		case PR:

			break;

		default: return;
	}
}






