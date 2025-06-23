/*
 * timer.h
 *
 *  Created on: Jun 20, 2025
 *      Author: dobao
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stdio.h>
#include "registerAddress.h"

typedef enum{
	my_TIM1, //0
	my_TIM2,
	my_TIM3,
	my_TIM4,
	my_TIM5,
	my_TIM9,
	my_TIM10,
	my_TIM11 //10
}TIM_Name_t;

typedef enum{
	TIM_CR1,
	TIM_CR2,
	TIM_SMCR,
	TIM_DIER,
	TIM_SR,

	TIM_EGR,
	TIM_CCMR1,
	TIM_CCMR2,
	TIM_CCER,
	TIM_CNT,

	TIM_PSC,
	TIM_ARR,
	TIM_RCR,

	TIM_CCR1,
	TIM_CCR2,
	TIM_CCR3,
	TIM_CCR4,

	TIM_BDTR,

	TIM_DCR,
	TIM_DMAR,
	TIM_OR
}TIM_Mode_t;


/*
 * Function declarations
 */
void writeTimer(uint8_t bitPosition, TIM_Name_t userTIMx, TIM_Mode_t mode, uint32_t value);

uint32_t readTimer (uint8_t bitPosiion, TIM_Name_t userTIMx, TIM_Mode_t mode);

#endif /* INC_TIMER_H_ */
