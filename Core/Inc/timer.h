/*
 * @file	timer.h
 * @brief	Basic TIMER utilities for STM32F411VET6 (TIM1 - TIM11)
 * 			Provide PSC/ARR calculations and safe register access helpers.
 *
 *  Created on: Jun 20, 2025
 *      Author: dobao
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

/*
 * -----------------------------------------------
 * Standard & Device Headers
 * -----------------------------------------------
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "registerAddress.h"
#include "exti.h"
#include "stm32f4xx.h"
#include "rcc.h"

/*
 * --------------------------------------------------
 * Clock Parameters Definitions
 * --------------------------------------------------
 */
#define NORMAL_SYSCLK_FREQ 	16000000	//16MHz SYSCLK Freq
#define FAST_SYSCLK_FREQ 	100000000 //100MHz SYSCLK Freq

#define _1MS_PER_TICK	0.001	//1msec per tick
#define _5MS_PER_TICK	0.005	//5msec per tick

#define TICK_FREQ_1000Hz	1/_1MS_PER_TICK //1ms per period
#define TICK_FREQ_200Hz		1/_5MS_PER_TICK //5ms per period


/*
 * ---------------------------------------------------
 * Enumerations
 * ---------------------------------------------------
 */

/*
 * @enum	TIM_Name_t
 * @brief	Logical indices for the timers we use
 */
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

/*
 * @enum	TIM_Mode_t
 * @brief	Individual timer registers
 */
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

	TIM2_OR,
	TIM5_OR,
	TIM11_OR
}TIM_Mode_t;

/*
 * ------------------------------------------------------
 * PSC/ARR Calculation Helper
 * ------------------------------------------------------
 */

/*
 * @struct	TIM_Cal_t
 * @brief	Result of timer prescaler/auto-reload computation
 */
typedef struct{
	uint32_t psc;	//Prescaler value
	uint32_t arr;	//Auto-reload Value
	uint32_t actualHz;	//Achieved frequency
}TIM_Cal_t;


/*
 * --------------------------------------------------------
 * Public API
 * --------------------------------------------------------
 */
void initTimer(TIM_Name_t userTIMx);
void delay(int msec);

void TIM1_UP_TIM10_IRQHandler();

void writeTimer(uint8_t bitPosition, TIM_Name_t userTIMx, TIM_Mode_t mode, uint32_t value);

uint32_t readTimer (uint8_t bitPosiion, TIM_Name_t userTIMx, TIM_Mode_t mode);

extern uint32_t readBits(volatile uint32_t* reg, uint8_t bitPosition, uint8_t bitWidth);

#endif /* INC_TIMER_H_ */
