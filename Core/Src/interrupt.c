/*
 * interrupt.c
 *
 *  Created on: Apr 25, 2025
 *      Author: dobao
 */
#include <stdint.h>

#include "registerAddress.h"
#include "gpio_write_read.h"
#include "interrupt.h"

/*
 * Global parameter declaration
 */
uint32_t* global_desiredOffsetAddr;


/*
 * @brief  Select which signal edge(s) will fire the EXTI interrupt.
 *
 * The STM32 EXTI block lets you generate an interrupt on a **rising edge**,
 * a **falling edge**, or on **both**.
 * This helper sets the appropriate bit in:
 *   • EXTI->RTSR (Rising-Trigger Selection Register)
 *   • EXTI->FTSR (Falling-Trigger Selection Register)
 *
 * @param bitPosition   EXTI line number you want to configure (0 – 15).
 * @param triggerMode   Choose one of:
 *                      - my_EXTI_TRIGGER_RISING   → interrupt on rising edge
 *                      - my_EXTI_TRIGGER_FALLING  → interrupt on falling edge
 *                      - my_EXTI_TRIGGER_BOTH     → interrupt on both edges
 *
 * The function quietly returns if an invalid mode is supplied.
 */
void EXTI_TriggerConfig(char bitPosition, EXTI_Trigger_t triggerMode){
	if(triggerMode == my_EXTI_TRIGGER_RISING || triggerMode == my_EXTI_TRIGGER_BOTH){
		WriteEXTI(bitPosition, RTSR, my_GPIO_PIN_SET); //Rising Edge Reg Enable
	}
	if(triggerMode == my_EXTI_TRIGGER_FALLING || triggerMode == my_EXTI_TRIGGER_BOTH){
		WriteEXTI(bitPosition, FTSR, my_GPIO_PIN_SET); //Falling Edge Reg Enable
	}
	else{
		return;
	}
}



/*
 * @brief  Enable a given EXTI line and its corresponding NVIC interrupt.
 *
 * This helper does two things:
 *   1. Unmasks the selected EXTI line by setting its bit in EXTI->IMR.
 *   2. Activates the matching IRQ in the NVIC by writing to the supplied
 *      NVIC_ISER register.
 *
 * @param 	bitPosition  	EXTI line number you want to enable (0 – 15).
 * @param 	NVIC_ISERx   	Pointer to NVIC_ISER0 … NVIC_ISER7 that holds the
 *                     		interrupt-enable bit for this EXTI line.
 *
 * @param 	EXTIx        	Enumeration value of the IRQ position inside the NVIC
 *                     		(e.g. EXTI0 = 6, EXTI1 = 7, …).  This is **not** the
 *                     		GPIO pin number but the vector-table index used by NVIC.
 */
void EXTI_Init(char bitPosition, volatile uint32_t* NVIC_ISERx, IQRn_User_t EXTIx){
	WriteEXTI(bitPosition, IMR, my_GPIO_PIN_SET);  //Interrupt Mask Reg Enable
	*NVIC_ISERx |= (1 << EXTIx); //Enable NVIC IRQ
}



/*
 * Relocate the Vector Table to a new memmory address
 *
 * @param 	desiredOffsetAddr: Pointer to the new memory location where the VT vill be copied and relocated
 *
 * This function:
 * 1.	Copies the content from the original Vector Table (pointed by VTO_REG)
 * 2.	Updates VTO_REG to point to the new location
 *
 * Note:
 * 		Both original and new vector tables are treated as arrays of uint32_t entries
 * 		Each pointer increment (+i) moves 4 bytes automatically because of uint32_t* pointer arithmetic.
 * 		The vector table size (0x198 words) cvers all standard exception and interrupt entries.
 */
void EXTI_Offset_Init(uint32_t* desiredOffsetAddr){
	uint32_t* originalVectorTable = (uint32_t*)*VTO_REG; //Get the original Vector Table address 0x00
	uint32_t* offsetVectorTable = desiredOffsetAddr; //Destination address for new VT

	for(int i = 0; i < 0x198; i++){
		*(offsetVectorTable + i) = *(originalVectorTable + i); //Copy each vector entry
	}
	*VTO_REG = (uint32_t)desiredOffsetAddr; //Update VTO_REG to point to the new VT location
	global_desiredOffsetAddr = desiredOffsetAddr;
}



/*
 * @brief  Dynamically assign a user-defined callback function to an EXTI interrupt.
 *
 * This function patches the vector table entry at runtime to point to a user-provided
 * callback function. It calculates the correct vector table entry based on the IRQ number.
 *
 * @param functionCallBack  Pointer to a user-defined function (void)(void)
 *                          that will be executed when the corresponding EXTI interrupt occurs.
 * @param IRQNumber         NVIC IRQ number (not EXTI line number). For example:
 *                          - 6  → EXTI0_IRQn
 *                          - 7  → EXTI1_IRQn
 *                          - 8  → EXTI2_IRQn
 *                          (see IRQn_Type enumeration for your STM32F4)
 *
 * Notes:
 * - Assumes the vector table has already been relocated to SRAM (e.g., 0x20000000).
 * - Vector Table Offset = 4 × IRQNumber (each vector table entry is 4 bytes).
 * - Writing to the wrong location may corrupt interrupt behavior — use carefully!
 */
void user_EXTI_IRQHandler(void(*functionCallBack)(void), uint8_t IRQNumber){
	void (**fncPointer)(void);
	fncPointer = (void (**)(void)) ((uintptr_t)global_desiredOffsetAddr + IRQNumber);
//	fncPointer = (void (**)(void)) 0x20000058 ;
	*fncPointer = functionCallBack;
}




