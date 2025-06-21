/*
 * timer.c
 *
 *  Created on: Jun 20, 2025
 *      Author: dobao
 */
#include "timer.h"








/*
 * @brief	Helper function to manipulate/write bit to specific bit position in TIMER's registers
 *
 * @param
 */
void writeTimer(uint8_t bitPosition, TIM_Name_t userTIMx, TIM_Mode_t mode, uint32_t value){
	if(bitPosition > 31) return;

	TIM_Register_Offset_t* TIMx_p; //Timer pointer
	switch(userTIMx){
		case my_TIM1: TIMx_p = TIM1_REG; break;
		case my_TIM2: TIMx_p = TIM2_REG; break;
		case my_TIM3: TIMx_p = TIM3_REG; break;
		case my_TIM4: TIMx_p = TIM4_REG; break;
		case my_TIM5: TIMx_p = TIM5_REG; break;
		case my_TIM9: TIMx_p = TIM9_REG; break;
		case my_TIM10: TIMx_p = TIM10_REG; break;
		case my_TIM11: TIMx_p = TIM11_REG; break;
		default: return; //Invalid TIMER
	}

	volatile uint32_t* reg;

	switch(mode){
		case TIM_CR1:
			if(userTIMx >= my_TIM1 && userTIMx <= my_TIM5 && bitPosition <= 9 ||
			   userTIMx >= my_TIM9 && bitPosition <= 9 && !(bitPosition >= 4 && bitPosition <= 6)){
				reg = &TIMx_p -> TIM_CR1; //Storing addr of that reg field in reg
			}
			else{
				return;
			}
		break;

		case TIM_CR2:

			reg = &TIMx_p -> TIM_CR2; break;
		case TIM_SMCR:	reg = &TIMx_p -> TIM_SMCR; break;
		case TIM_DIER:	reg = &TIMx_p -> TIM_DIER; break;
		case TIM_SR:	reg = &TIMx_p -> TIM_SR; break;

		case TIM_EGR:	reg = &TIMx_p -> TIM_EGR; break;
		case TIM_CCMR1:	reg = &TIMx_p -> TIM_CCMR1; break;
		case TIM_CCMR2:	reg = &TIMx_p -> TIM_CCMR2; break;
		case TIM_CCER:	reg = &TIMx_p -> TIM_CCER; break;
		case TIM_CNT:	reg = &TIMx_p -> TIM_CNT; break;

		case TIM_PSC:	reg = &TIMx_p -> TIM_PSC; break;
		case TIM_ARR:	reg = &TIMx_p -> TIM_ARR; break;
		case TIM_RCR:	reg = &TIMx_p -> TIM_RCR; break;

		case TIM_CCR1:	reg = &TIMx_p -> TIM_CCR1; break;
		case TIM_CCR2:	reg = &TIMx_p -> TIM_CCR2; break;
		case TIM_CCR3:	reg = &TIMx_p -> TIM_CCR3; break;
		case TIM_CCR4:	reg = &TIMx_p -> TIM_CCR4; break;

		case TIM_BDTR:	reg = &TIMx_p -> TIM_BDTR; break;
		case TIM_DCR:	reg = &TIMx_p -> TIM_DCR; break;
		case TIM_DMAR:	reg = &TIMx_p -> TIM_DMAR; break;
		case TIM_OR:	reg = &TIMx_p -> TIM_OR; break;

		default: return;
	}

	//Auto detect bitwidth based on the "value" length
	uint32_t bitWidth = 0;
	uint32_t temp = value;

	while(temp > 0){
		bitWidth++;
		temp = temp >> 1;
	}

	if (value == 0 && bitWidth == 0) bitWidth = 1;
	if(bitPosition + bitWidth > 32) return; //prevent overflow

	//Mask off the old bit and or with the new bit
	uint32_t mask = ((1U << bitWidth) -1U) << bitPosition;
	uint32_t shiftedValue = (value << bitPosition) & mask;
	*reg = (*reg & ~mask) | shiftedValue;
}



