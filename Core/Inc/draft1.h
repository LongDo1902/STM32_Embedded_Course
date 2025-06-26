/*
 * interrupt.h
 *
 *  Created on: Apr 25, 2025
 *      Author: dobao
 */

#ifndef INC_INTERRUPT_H_
#define INC_INTERRUPT_H_

#include <stdint.h>
#include "registerAddress.h"
#include "gpio_write_read.h"

typedef enum {
	my_EXTI_TRIGGER_RISING,
	my_EXTI_TRIGGER_FALLING,
	my_EXTI_TRIGGER_BOTH
}EXTI_Trigger_t;



/*
 * Function declarations
 */
void EXTI_TriggerConfig(char bitPosition, EXTI_Trigger_t triggerMode);

void EXTI_Init(char bitPosition, volatile uint32_t* NVIC_ISERx, IQRn_User_t EXTIx);

void EXTI_Offset_Init(uint32_t* desiredOffsetAddr);

void user_EXTI_IRQHandler(void(*functionCallBack)(void), uint8_t IRQNumber);

void writeEXTI(uint8_t bitPosition,
			   EXTI_Mode_t mode,
			   GPIO_State_t state);

#endif /* INC_INTERRUPT_H_ */
