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

	//Due to the nature of CCMR register, which has input (capture mode) and output (compare mode)
	//Same bits but different mode(input/output) will have different functions
	if(mode == TIM_CCMR1 || mode == TIM_CCMR2){
		printf("Use writeCCMR() instead.\n");
		return;
	}

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
		/*
		 * TIMER1 to TIMER5 have valid bits <= 9
		 * TIMER9 to TIMER11 have valid bits <= 9 but not 4 to 6
		 */
		case TIM_CR1:
			if((userTIMx >= my_TIM1 && userTIMx <= my_TIM5 && bitPosition <= 9) ||
			   (userTIMx >= my_TIM9 && bitPosition <= 9 && !(bitPosition >= 4 && bitPosition <= 6))){
				reg = &TIMx_p -> TIM_CR1; //Storing addr of that reg field in reg
			}
			else{
				return;
			}
		break;


		/*
		 * Only TIMER1 to TIMER5 have CR2 register
		 * These bits are valid when not equal to 1 and 15
		 */
		case TIM_CR2:
			if(userTIMx >= my_TIM1 && userTIMx <= my_TIM5 && bitPosition != 1 && bitPosition != 15){
				reg = &TIMx_p -> TIM_CR2;
			}
			else{
				return;
			}
		break;


		/*
		 * Only TIMER1 to TIMER5 and TIMER 9 have SMCR register
		 * but:
		 * 		TIMER1 to TIMER5 have reserved bit at bit position 3
		 * 		TIMER9 has reserved bits at 3 and bits larger 7
		 */
		case TIM_SMCR:
			if((userTIMx >= my_TIM1 && userTIMx <= my_TIM5 && bitPosition != 3) ||
				(userTIMx == my_TIM9 && bitPosition != 3 && bitPosition <= 7)){
				reg = &TIMx_p -> TIM_SMCR;
			}
			else{
				return;
			}
		break;


		/*
		 * TIMER1 has reversed bit at 15
		 * TIMER2 to TIMER5 have reserved bits at 5, 7, 13, 15
		 * TIMER9 has valid bits within 0 to 2 and at bit 6
		 * TIMER10 to TIMER11 have valid bits less than bit 2
		 */
		case TIM_DIER:
			if((userTIMx == my_TIM1 && bitPosition != 15) ||
			   ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && bitPosition != 5 && bitPosition != 7 && bitPosition != 13 && bitPosition != 15) ||
			   (userTIMx == my_TIM9 && (bitPosition <= 2 || bitPosition == 6)) ||
			   ((userTIMx == my_TIM10 || userTIMx == my_TIM11) && bitPosition <= 1)){
				reg = &TIMx_p -> TIM_DIER;
			}
			else{
				return;
			}
		break;


		/*
		 * TIMER1 has reserved bits at 8 and at bits that larger than 12
		 * TIMER2 to TIMER5 has valid bits smaller than it 13 but except 5, 7, 8
		 * TIMER9 to TIMER11 have valid bits smaller than 11 but not in range 3 to 5 and 7 and 8
		 */
		case TIM_SR:
			if((userTIMx == my_TIM1 && bitPosition <= 12 && bitPosition != 8) ||
			   ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && bitPosition <= 12 && (bitPosition != 5 && bitPosition != 7 && bitPosition != 8)) ||
			   ((userTIMx >= my_TIM9 && userTIMx <= my_TIM11) && bitPosition <= 10 && (!(bitPosition >= 3 && bitPosition <= 5) && bitPosition != 7 && bitPosition != 8))){
				reg = &TIMx_p -> TIM_SR;
			}
			else{
				return;
			}
			break;


		/*
		 *
		 */
		case TIM_EGR:
			if((userTIMx == my_TIM1 && bitPosition <= 7) ||
			  ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && bitPosition <= 6 && bitPosition != 5) ||
			  ((userTIMx == my_TIM10 || userTIMx == my_TIM11) && bitPosition <= 1)){
				reg = &TIMx_p -> TIM_EGR;
			}
			else{
				return;
			}
			break;

		case TIM_CCER:
			if((userTIMx == my_TIM1 && bitPosition != 14) ||
			   ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && bitPosition != 14 && bitPosition != 10 && bitPosition != 6 && bitPosition != 2) ||
			   ((userTIMx == my_TIM9 && bitPosition <= 7 && bitPosition != 6 && bitPosition != 2) ||
			   ((userTIMx == my_TIM10 || userTIMx == my_TIM11) && bitPosition <= 3 && bitPosition != 2))){
				reg = &TIMx_p -> TIM_CCER;
			}

			else{
				return;
			}
			break;

		case TIM_CNT:	reg = &TIMx_p -> TIM_CNT; break; //No reserved bits
		case TIM_PSC:	reg = &TIMx_p -> TIM_PSC; break; //No reserved bits
		case TIM_ARR:	reg = &TIMx_p -> TIM_ARR; break; //No reserved bits

		case TIM_RCR: //Only available in TIMER1
			if(userTIMx == my_TIM1 && bitPosition <= 7){
				reg = &TIMx_p -> TIM_RCR;
			}
			else{
				return;
			}
			break;

		case TIM_CCR1: reg = &TIMx_p -> TIM_CCR1; break; //All TIMERs have CCR1

		case TIM_CCR2:
			if(!(userTIMx == my_TIM10 || userTIMx == my_TIM11)){ //Only TIMER1/2/3/4/5/9 have CCR2
				reg = &TIMx_p -> TIM_CCR2;
			}
			else{
				return;
			}
		break;

		case TIM_CCR3:
			if(!(userTIMx >= my_TIM9)){ //Only TIMER1/2/3/4/5 have CCR3
				reg = &TIMx_p -> TIM_CCR3;
			}
			else{
				return;
			}
		break;

		case TIM_CCR4:
			if(!(userTIMx >= my_TIM9)){ //Only TIMER1/2/3/4/5 have CCR4
				reg = &TIMx_p -> TIM_CCR4;
			}
			else{
				return;
			}
		break;

		case TIM_BDTR:
			if(userTIMx == my_TIM1){
				reg = &TIMx_p -> TIM_BDTR;
			}
			else{
				return;
			}
		break;

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



