/*
 * interrupts.h
 *
 *  Created on: Apr 5, 2025
 *      Author: dobao
 */

#ifndef INC_INTERRUPTS_H_
#define INC_INTERRUPTS_H_

//Function declaration
void EXTI0_Init();
void redirect_EXTI0_IRQHandler(); //Redirect and customize predefined EXTI0_IRQHandler
void user_EXTI0_IRQHandler();

#endif /* INC_INTERRUPTS_H_ */
