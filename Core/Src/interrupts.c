/*
 * interrupts.c
 *
 *  Created on: Apr 5, 2025
 *      Author: dobao
 */
#include "interrupts.h"
#include "registerAddress.h"
#include "LEDs.h"
#include "buttons.h"

/*
 * Function to initialize EXTI0 (External Interrupt 0)
 */
void EXTI0_Init(){
	EXTI_REG -> RTSR |= (1 << 0);
	EXTI_REG -> FTSR |= (1 << 0);
	EXTI_REG -> IMR |= (1 << 0);
	*NVIC_ISER0 |= (1 << 6);
}


/*
 * Redirect the EXTI0_IRQHandler to a new address 0x20000058
 */
void redirect_EXTI0_IRQHandler(){
	//Declare a pointer (fncPointer) it stores the offset of address of EXTI0_IRQHandler function
	uint32_t* fncPointer =(uint32_t*)(SRAM_ADDR_DEFAULT + 0x58); //0x20000058
	*fncPointer = (uint32_t) user_EXTI0_IRQHandler; //Assign fnc addr to memory space of
}


/*
 * Customized EXTI0_IRQHandler
 */
void user_EXTI0_IRQHandler(){
	if(buttonState()){
		LED_Control(LED_Green, 1);
	}
	else{
		LED_Control(LED_Green, 0);
	}
	EXTI_REG -> PR = (1 << 0); //clear the interrupt flag
}


