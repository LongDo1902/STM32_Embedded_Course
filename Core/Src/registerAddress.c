/*
 * registerAddress.c
 *
 *  Created on: Apr 5, 2025
 *      Author: dobao
 */

#include "registerAddress.h"


/*
 * Intialize EXTIx (External Interrupt x)
 * @brief: this is a general function to initialize any pin, EXTI, NVIC_ISERx
 * @param pin is pin number of interrupt
 * @param NVIC_ISERx where x is 0 to 7
 * @param EXTIx where x is 0 to 4
 */
void EXTI_Init(char pin, NVIC_ISER_t NVIC_ISERx, EXTI_Num_t EXTIx){
	EXTI_REG -> RTSR |= (1 << pin);
	EXTI_REG -> FTSR |= (1 << pin);
	EXTI_REG -> IMR |= (1 << pin);
	*((volatile uint32_t*) NVIC_ISERx) |= (1 << EXTIx);
}


/*
 * Offset the whole vector table to a RAM (0x20000000)
 */
void EXTI_Offset_Init(){
	uint8_t* originalVectorTable = 0x00; //Original vector table table first located at 0x00000000
	uint8_t* offsetVectorTable = (uint8_t*)0x20000000; //Offset target vector table 0x20000000 (the first addr of RAM)

	for(int i = 0; i < 0x198; i++){
		*(offsetVectorTable + i) = *(originalVectorTable + i); //Copy to the new offset vector table
	}
	uint32_t* VTO_REG = (uint32_t*) 0xE000ED08;

	*VTO_REG = 0x20000000;
}


/*
 *
 */
void user_EXTI_IRQHandler(void(*functionCallBack)(void)){
	void (**fncPointer)(void);
	fncPointer = (void (**)(void)) 0x20000058;
	*fncPointer = functionCallBack;
}






