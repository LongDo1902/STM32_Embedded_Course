/*
 * timer.h
 *
 *  Created on: Jun 20, 2025
 *      Author: dobao
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stdio.h>
#include <stdbool.h>
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

	TIM2_OR,
	TIM5_OR,
	TIM11_OR
}TIM_Mode_t;


/*
 * Function declarations
 */
void writeTimer(uint8_t bitPosition, TIM_Name_t userTIMx, TIM_Mode_t mode, uint32_t value);

uint32_t readTimer (uint8_t bitPosiion, TIM_Name_t userTIMx, TIM_Mode_t mode);


static inline bool isValidTimerBit(uint8_t bitPosition, TIM_Name_t userTIMx, TIM_Mode_t mode){
	switch(mode){
		/*
		 * TIMx_CR1:
		 * 		TIM1 to TIM5: valid bits are [0:9]
		 * 		TIM9 to TIM11: valid bits are [0:9] excluding [4:6]
		 */
		case TIM_CR1:
			return ((userTIMx >= my_TIM1 && userTIMx <= my_TIM5) && bitPosition < 10) ||
				   ((userTIMx >= my_TIM9 && userTIMx <= my_TIM11) && bitPosition < 10 && !(bitPosition >= 4 && bitPosition <= 6));
		break;

		/*
		 * TIMx_CR2:
		 * 		TIM1: reserved bits at 1 and 15
		 * 		TIM2 to TIM5: valid bits at [3:7]
		 * 		TIM9 to TIM11 has no CR2
		 */
		case TIM_CR2:
			return (userTIMx == my_TIM1 && bitPosition != 1 && bitPosition != 15) ||
				   ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && (bitPosition >= 3 && bitPosition <= 7));
		break;


		/*
		 * TIMx_SMCR
		 * 		TIM1 to TIM5: reserved bits at 3
		 * 		TIM9: reserved at bits 3 and [8:15]
		 * 		TIM10 to TIM11 does not have TIMx_SMCR
		 */
		case TIM_SMCR:
			return ((userTIMx >= my_TIM1 && userTIMx <= my_TIM5) && bitPosition != 3) ||
					(userTIMx == my_TIM9 && bitPosition < 8 && bitPosition != 3);
		break;


		/*
		 * TIMx_DIER
		 * 		TIM1: reserved bit at 15
		 * 		TIM2 to TIM5: reserved bits at 15, 13, 7, 5
		 * 		TIM9: reserved bits within [3:5] and [7:15]
		 * 		TIM10 to TIM11: valid bits at 0 and 1
		 */
		case TIM_DIER:
			return (userTIMx == my_TIM1 && bitPosition != 15) ||
				  ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && bitPosition != 15 && bitPosition != 13 && bitPosition != 7 && bitPosition != 5) ||
				   (userTIMx == my_TIM9 && !(bitPosition >= 3 && bitPosition <= 5) && !(bitPosition >= 7 && bitPosition <= 15)) ||
				  ((userTIMx == my_TIM10 || userTIMx == my_TIM11) && (bitPosition == 0 || bitPosition == 1));
		break;


		/*
		 * TIMx_SR
		 * 		TIM1: reserved bits at [13:15] and 8
		 * 		TIM2 to TIM5: reserved bits at [13:15], [7:8], and 5
		 * 		TIM9: reserved bits at [11:15], [7:8] and [3:5]
		 * 		TIM10 to TIM11: valid bits at 0, 1, 9
		 */
		case TIM_SR:
			return(userTIMx == my_TIM1 && !(bitPosition >= 13 && bitPosition <= 15) && bitPosition != 8) ||
				 ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && !(bitPosition >= 13 && bitPosition <= 15) && bitPosition != 8 && bitPosition != 7 && bitPosition != 5) ||
				  (userTIMx == my_TIM9 && !(bitPosition >= 11 && bitPosition <= 15) && bitPosition != 8 && bitPosition != 7 && !(bitPosition >= 3 && bitPosition <= 5)) ||
				 ((userTIMx == my_TIM10 || userTIMx == my_TIM11) && (bitPosition == 9 || bitPosition == 1 || bitPosition == 0));
		break;


		/*
		 * TIMx_EGR
		 * 		TIM1: valid bits [0:7]
		 * 		TIM2 to TIM5: valid bits [0:6] excluding bit 5
		 * 		TIM9: valid bits at 6 and [0:2]
		 * 		TIM10 to TIM11: valid bits at [0:1]
		 */
		case TIM_EGR:
			return (userTIMx == my_TIM1 && (bitPosition >= 0 && bitPosition <= 7)) ||
				  ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && (bitPosition >= 0 && bitPosition <= 6) && bitPosition != 5) ||
				   (userTIMx == my_TIM9 && (bitPosition == 6 || (bitPosition >= 0 && bitPosition <= 2))) ||
				  ((userTIMx == my_TIM10 || userTIMx == my_TIM11) && (bitPosition == 1 || bitPosition == 0));
		break;


		/*
		 * TIMx_CCER
		 * 		TIM1: valid bits [0:13]
		 * 		TIM2 to TIM5: reserved bits at 14, 10, 6, 2
		 * 		TIM9: reserved bits at [8:15], 6, 2
		 * 		TIM10 to TIM11: valid bits at 3, 1, 0
		 */
		case TIM_CCER:
			return (userTIMx == my_TIM1 && (bitPosition >= 0 && bitPosition <= 13)) ||
				  ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && bitPosition != 14 && bitPosition != 10 && bitPosition != 6 && bitPosition != 2) ||
				   (userTIMx == my_TIM9 && bitPosition < 8 && bitPosition != 6 && bitPosition != 2) ||
				  ((userTIMx == my_TIM10 || userTIMx == my_TIM11) && (bitPosition == 3 || bitPosition == 1 || bitPosition == 0));
		break;


		/*
		 * TIMx_CNT
		 */
		case TIM_CNT:
			return bitPosition == 0;
		break;


		/*
		 * TIMx_PSC
		 */
		case TIM_PSC:
			return bitPosition == 0;
		break;


		/*
		 * TIMx_ARR
		 */
		case TIM_ARR:
			return bitPosition == 0;
		break;


		/*
		 * TIMx_RCR
		 * 		Only available in TIM1
		 * 		TIM1: valid bits [0:7]
		 */
		case TIM_RCR:
			return userTIMx == my_TIM1 && bitPosition <= 7;
		break;


		/*
		 * TIMx_CCR1 is available for all timers
		 */
		case TIM_CCR1:
			return bitPosition == 0;
		break;


		/*
		 * TIMx_CCR2 is not available on TIM10 and TIM11
		 */
		case TIM_CCR2:
			return (userTIMx >= my_TIM1 && userTIMx <= my_TIM9) && bitPosition == 0;
		break;


		/*
		 * TIMx_CCR3 is only available on TIM1 to TIM5
		 */
		case TIM_CCR3:
			return (userTIMx >= my_TIM1 && userTIMx <= my_TIM5) && bitPosition == 0;
		break;


		/*
		 * TIMx_CCR4 is only available on TIM1 to TIM5
		 */
		case TIM_CCR4:
			return (userTIMx >= my_TIM1 && userTIMx <= my_TIM5) && bitPosition == 0;
		break;


		/*
		 * TIMx_BDTR is only available on TIM1
		 */
		case TIM_BDTR:
			return userTIMx == my_TIM1;
		break;


		/*
		 * TIMx_DCR
		 * 		Only available on TIM1 to TIM5
		 * 		All have reserved bits in [5:7] and [13:15]
		 */
		case TIM_DCR:
			return (userTIMx >= my_TIM1 && userTIMx <= my_TIM5) && !(bitPosition >= 5 && bitPosition <= 7) && !(bitPosition >= 13 && bitPosition <= 15);
		break;


		/*
		 * TIMx_DMAR
		 * 		Only available in TIM1 to TIM5
		 */
		case TIM_DMAR:
			return (userTIMx >= my_TIM1 && userTIMx <= my_TIM5) && bitPosition == 0;
		break;


		/*
		 * TIM2_OR
		 * 		TIM2: valid bits at 11 and 10
		 */
		case TIM2_OR:
			return userTIMx == my_TIM2 && bitPosition == 10;
		break;


		/*
		 * TIM5_OR
		 * 		TIM5: valid bits at 7 and 6
		 */
		case TIM5_OR:
			return userTIMx == my_TIM5 && bitPosition == 6;
		break;


		/*
		 * TIM11_OR
		 * 		TIM11: valid bits at 1 and 0
		 */
		case TIM11_OR:
			return userTIMx == my_TIM11 && bitPosition == 0;
		break;

		default: return false;
	}
}

#endif /* INC_TIMER_H_ */
