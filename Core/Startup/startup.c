/*
 * startup.c
 *
 *  Created on: Jul 14, 2025
 *      Author: dobao
 *
 *  Purpose:
 *  	* Provide the Cortex-M4 vector table
 *  	* Initialize .data and .bss sections before jumping to main()
 */

#include <stdint.h>

int main();

/*
 * -------------------------------------------------------
 * Forward Declarations
 * -------------------------------------------------------
 */
void TIM1_UP_TIM10_IRQHandler();
typedef void(*handler_t)();

/*
 * -------------------------------------------------------
 * Symbols exported by the linker script
 * -------------------------------------------------------
 */
extern uint32_t _estack; 	//End of stack (End of RAM addr / start of FLASH addr)
extern uint32_t _sbss;		//Start of .bss (bss is a place to store uinitialized parameters)
extern uint32_t _ebss; 		//End of .bss
extern uint32_t _sidata; 	//Start of init values for .data(in FLASH)
extern uint32_t _sdata;		//Start of .data(SRAM)
extern uint32_t _edata; 	//End of .data(SRAM)

/*
 * -------------------------------------------------------
 * Main Helper Function
 * -------------------------------------------------------
 */
void resetHandler(){
	/* Clear all uninitialized global parameters - bss */
	uint32_t* bss_start = &_sbss;
	uint32_t* bss_end = &_ebss;

	while(bss_start < bss_end){
		*bss_start = 0; //Asign zero to all uninitialized global parameters
		bss_start++;
	}

	/* Pass/copy data from sidata (FLASH) to sdata(RAM) for data initialization */
	uint32_t* data_start = &_sdata;
	uint32_t* data_end = &_edata;
	uint32_t* idata_start = &_sidata;
	while(data_start < data_end){
		*data_start = *idata_start;
		data_start++;
		idata_start++;
	}

	main();
}

__attribute__((section(".isr_vector"))) handler_t VTTB[] = {
		//_estack = ORIGIN(RAM) + LENGTH(RAM); /* end of "RAM" Ram type memory */
		(handler_t)&_estack,
		resetHandler,
		0,0,0,0,

		0,0,0,0,

		0,0,0,0,

		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,
		TIM1_UP_TIM10_IRQHandler,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
