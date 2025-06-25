/*
 * timer.c
 *
 *  Created on: Jun 20, 2025
 *      Author: dobao
 */

#include "timer.h"


/*
 * @brief	Writes a bit field into a specific TIM register at a given position,
 * 			only if the register and bit are valid for the selected TIM peripheral
 *
 * @param	bitPosition		The starting bit position in the target register
 * @param	userTIMx		The TIM peripheral (e.g., my_TIM1, my_TIM5, etc.)
 * @param	mode			The specific register being written (e.g., TIM_CR1, TIM_EGR, etc.)
 * @param	value			The value to write into the bit field
 */
void writeTimer(uint8_t bitPosition, TIM_Name_t userTIMx, TIM_Mode_t mode, uint32_t value){
	if(bitPosition > 31) return;

	//Due to the nature of CCMR register, which has input (capture mode) and output (compare mode)
	//Same bits but different mode(input/output) will have different functions
	if(mode == TIM_CCMR1 || mode == TIM_CCMR2){
		printf("Use writeCCMR() instead.\n");
		return;
	}

	TIM_Register_Offset_t* TIMx_p; //Timer Pointer

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
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CR1)){
				reg = &TIMx_p -> TIM_CR1;
			} else return;
			break;


		case TIM_CR2:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CR2)){
				reg = &TIMx_p -> TIM_CR2;
			} else return;
			break;


		case TIM_SMCR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_SMCR)){
				reg = &TIMx_p -> TIM_SMCR;
			} else return;
			break;


		case TIM_DIER:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_DIER)){
				reg = &TIMx_p -> TIM_DIER;
			} else return;
			break;


		case TIM_SR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_SR)){
				reg = &TIMx_p -> TIM_SR;
			} else return;
			break;


		case TIM_EGR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_EGR)){
				reg = &TIMx_p -> TIM_EGR;
			} else return;
			break;


		case TIM_CCER:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CCER)){
				reg = &TIMx_p -> TIM_CCER;
			} else return;
			break;


		case TIM_CNT:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CNT)){
				reg = &TIMx_p -> TIM_CNT;
			} else return;
			break;


		case TIM_PSC:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_PSC)){
				reg = &TIMx_p -> TIM_PSC;
			} else return;
			break;


		case TIM_ARR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_ARR)){
				reg = &TIMx_p -> TIM_ARR;
			} else return;
			break;


		case TIM_RCR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_RCR)){
				reg = &TIMx_p -> TIM_RCR;
			} else return;
			break;


		case TIM_CCR1:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CCR1)){
				reg = &TIMx_p -> TIM_CCR1;
			} else return;
			break;


		case TIM_CCR2:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CCR2)){
				reg = &TIMx_p -> TIM_CCR2;
			} else return;
			break;


		case TIM_CCR3:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CCR3)){
				reg = &TIMx_p -> TIM_CCR3;
			} else return;
			break;


		case TIM_CCR4:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CCR4)){
				reg = &TIMx_p -> TIM_CCR4;
			} else return;
			break;


		case TIM_BDTR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_BDTR)){
				reg = &TIMx_p -> TIM_BDTR;
			} else return;
			break;


		case TIM_DCR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_DCR)){
				reg = &TIMx_p -> TIM_DCR;
			} else return;
			break;


		case TIM_DMAR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_DMAR)){
				reg = &TIMx_p -> TIM_DMAR;
			} else return;
			break;


		case TIM2_OR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM2_OR)){
				reg = &TIMx_p -> TIM2_OR;
			} else return;
			break;


		case TIM5_OR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM5_OR)){
				reg = &TIMx_p -> TIM5_OR;
			} else return;
			break;


		case TIM11_OR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM11_OR)){
				reg = &TIMx_p -> TIM11_OR;
			} else return;
			break;

		default: return;
	}

	//Auto detect bitWidth based on the "value" length
	uint32_t bitWidth = 0;
	uint32_t temp = value;

	while(temp > 0){
		bitWidth++;
		temp = temp >> 1;
	}

	if(value == 0 && bitWidth == 0) bitWidth = 1;
	if(bitPosition + bitWidth > 32) return; //Prevent overflow
	if(bitWidth == 32 && bitPosition != 0) return; //C does not allow shifting more than or equal to the size of the type

	//Mask off the old bit and OR with new bit
	uint32_t mask = ((1U << bitWidth) - 1U) << bitPosition;
	uint32_t shiftedValue = (value << bitPosition) & mask;
	*reg = (*reg & ~mask) | shiftedValue;
}



/*
 * @brief	Dynamically read bit(s) from a specific register of a selected TIM peripheral
 *
 * @param	bitPosition		Bit offset within the register (0-31)
 * @param	userTIMx		Selected timer (my_TIM1 to my_TIM11)
 * @param	mode			Register to read from
 *
 * @retval	The value of the selected bit field
 * @retval	0xFFFFFFFF (ERROR_FLAG) if invalid bit/register/timer combo
 */
uint32_t readTimer(uint8_t bitPosition, TIM_Name_t userTIMx, TIM_Mode_t mode){
	const uint32_t ERROR_FLAG = 0xFFFFFFFF;

	if(bitPosition > 31) return ERROR_FLAG;

	//Due to the nature of CCMR register, which has input (capture mode) and output (compare mode)
	//Same bits but different mode(input/output) will have different functions
	if(mode == TIM_CCMR1 || mode == TIM_CCMR2){
		printf("Use writeCCMR() instead.\n");
		return ERROR_FLAG;
	}

	volatile uint32_t* reg;
	TIM_Register_Offset_t* TIMx_p;

	switch(userTIMx){
		case my_TIM1: TIMx_p = TIM1_REG; break;
		case my_TIM2: TIMx_p = TIM2_REG; break;
		case my_TIM3: TIMx_p = TIM3_REG; break;
		case my_TIM4: TIMx_p = TIM4_REG; break;
		case my_TIM5: TIMx_p = TIM5_REG; break;
		case my_TIM9: TIMx_p = TIM9_REG; break;
		case my_TIM10: TIMx_p = TIM10_REG; break;
		case my_TIM11: TIMx_p = TIM11_REG; break;
		default: return ERROR_FLAG;
	}

	uint8_t bitWidth = 1; //Default 1-bit fields

	switch(mode){

		case TIM_CR1:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CR1)){
				reg = &TIMx_p -> TIM_CR1;
				/*
				 * TIM1 to TIM5:
				 * 		CMS[1:0] at bit 5
				 * 		CKD[1:0] at bit 8
				 *
				 * TIM9 to TIM11:
				 * 		CKD[1:0] at 8: bit 8 to 9 -> 2 bits
				 */

				if(((userTIMx >= my_TIM1 && userTIMx <= my_TIM5) && (bitPosition == 8 || bitPosition == 5)) ||
				   ((userTIMx >= my_TIM9 && userTIMx <= my_TIM11) && bitPosition == 8)){
					bitWidth = 2;
				}

				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM_CR2:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CR2)){
				reg = &TIMx_p -> TIM_CR2;
				/*
				 * TIM1 to TIM5:
				 * 		MMS[2:0] at bit 4
				 */
				if((userTIMx >= my_TIM1 && userTIMx <= my_TIM5) && bitPosition == 4){
					bitWidth = 3;
				}

				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM_SMCR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_SMCR)){
				reg = &TIMx_p -> TIM_SMCR;
				/*
				 * TIM1 to TIM5
				 * 		SMS[2:0] at bit 0
				 * 		TS[2:0] at bit 4
				 * 		ETF[3:0] at bit 8
				 * 		ETPS[1:0] at bit 12
				 * 		else 1 bit
				 * TIM9
				 * 		SMS[2:0] at bit 0
				 * 		TS[2:0] at bit 4
				 * 		else 1 bit
				 */
				if((userTIMx >= my_TIM1 && userTIMx <= my_TIM9) && (bitPosition == 0 || bitPosition == 4)){
					bitWidth = 3; //read 3 bits
				}
				else if((userTIMx >= my_TIM1 && userTIMx <= my_TIM5) && bitPosition == 8){
					bitWidth = 4; //read 4 bits
				}
				else if((userTIMx >= my_TIM1 && userTIMx <= my_TIM5) && bitPosition == 12){
					bitWidth = 2; //read 2 bits
				}

				return readBits(reg, bitPosition, bitWidth); //read 1 bit
			} else return ERROR_FLAG;
			break;


		case TIM_DIER:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_DIER)){
				reg = &TIMx_p -> TIM_DIER;
				/*
				 * TIM1 to TIM11 have 1 bit read
				 */
				return readBits(reg, bitPosition, bitWidth); //read 1 bit
			} else return ERROR_FLAG;
			break;


		case TIM_SR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_SR)){
				reg = &TIMx_p -> TIM_SR;
				/*
				 * TIM1 to TIM11 have 1 bit read
				 */
				return readBits(reg, bitPosition, bitWidth); //read 1 bit
			} else return ERROR_FLAG;
			break;


		case TIM_EGR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_EGR)){
				reg = &TIMx_p -> TIM_EGR;
				/*
				 * TIM1 to TIM11 have 1-bit read
				 */
				return readBits(reg, bitPosition, bitWidth); //read 1 bit
			} else return ERROR_FLAG;
			break;


		case TIM_CCER:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CCER)){
				reg = &TIMx_p -> TIM_CCER;
				/*
				 * TIM1 to TIM11 have 1-bit read
				 */
				return readBits(reg, bitPosition, bitWidth); //read 1 bit
			} else return ERROR_FLAG;
			break;


		case TIM_CNT:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CNT)){
				reg = &TIMx_p -> TIM_CNT;
				/*
				 * TIM1, TIM3, TIM4, TIM9, TIM10 and TIM11: 16-bits field
				 * TIM2 and TIM5: 32-bits field
				 */
				if(userTIMx == my_TIM1 || userTIMx == my_TIM3 || userTIMx == my_TIM4 ||
				   userTIMx == my_TIM9 || userTIMx == my_TIM10 || userTIMx == my_TIM11){
					bitWidth = 16;
				}
				else bitWidth = 32; //TIM2 and TIM5

				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM_PSC:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_PSC)){
				reg = &TIMx_p -> TIM_PSC;
				/*
				 * TIM1 to TIM11 have 16-bit read
				 */
				bitWidth = 16;
				return readBits(reg, bitPosition, bitWidth); //read 16 bits
			} else return ERROR_FLAG;
			break;


		case TIM_ARR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_ARR)){
				reg = &TIMx_p -> TIM_ARR;
				/*
				 * TIM1, TIM3, TIM4, TIM9, TIM10 and TIM11: 16-bits field
				 * TIM2 and TIM5: 32-bits field
				 */
				if(userTIMx == my_TIM1 || userTIMx == my_TIM3 || userTIMx == my_TIM4 ||
				   userTIMx == my_TIM9 || userTIMx == my_TIM10 || userTIMx == my_TIM11){
					bitWidth = 16;
				}
				else bitWidth = 32; //TIM2 and TIM5

				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM_CCR1:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CCR1)){
				reg = &TIMx_p -> TIM_CCR1;
				/*
				 * TIM1, TIM3, TIM4, TIM9, TIM10 and TIM11: 16-bits field
				 * TIM2 and TIM5: 32-bits field
				 */
				if(userTIMx == my_TIM1 || userTIMx == my_TIM3 || userTIMx == my_TIM4 ||
				   userTIMx == my_TIM9 || userTIMx == my_TIM10 || userTIMx == my_TIM11){
					bitWidth = 16;
				}
				else bitWidth = 32; //TIM2 and TIM5

				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM_CCR2:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CCR2)){
				reg = &TIMx_p -> TIM_CCR2;
				/*
				 * TIM1, TIM3, TIM4: 16-bits field
				 * TIM2 and TIM5: 32-bits field
				 */
				if(userTIMx == my_TIM1 || userTIMx == my_TIM3 || userTIMx == my_TIM4){
					bitWidth = 16;
				}
				else bitWidth = 32; //TIM2 and TIM5

				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM_CCR3:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CCR3)){
				reg = &TIMx_p -> TIM_CCR3;
				/*
				 * TIM1, TIM3, TIM4: 16-bits field
				 * TIM2 and TIM5: 32-bits field
				 */
				if(userTIMx == my_TIM1 || userTIMx == my_TIM3 || userTIMx == my_TIM4){
					bitWidth = 16;
				}
				else bitWidth = 32; //TIM2 and TIM5

				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM_CCR4:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_CCR4)){
				reg = &TIMx_p -> TIM_CCR4;
				/*
				 * TIM1, TIM3, TIM4: 16-bits field
				 * TIM2 and TIM5: 32-bits field
				 */
				if(userTIMx == my_TIM1 || userTIMx == my_TIM3 || userTIMx == my_TIM4){
					bitWidth = 16;
				}
				else bitWidth = 32; //TIM2 and TIM5

				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM_BDTR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_BDTR)){
				reg = &TIMx_p -> TIM_BDTR;
				//Only available on TIM1
				if(bitPosition == 0){
					bitWidth = 8; //DTG[7:0]
				}
				else if (bitPosition == 8){
					bitWidth = 2; //LOCK[1:0]
				}
				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM_DCR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_DCR)){
				reg = &TIMx_p -> TIM_DCR;
				/*
				 * TIM1 to TIM5
				 * 		DBA[4:0] at bit 0
				 * 		DBL[4:0] at bit 8
				 */
				if((userTIMx >= my_TIM1 && userTIMx <= my_TIM5) && (bitPosition == 0 || bitPosition == 8)){
					bitWidth = 5;
				}
				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM_DMAR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM_DMAR)){
				reg = &TIMx_p -> TIM_DMAR;
				/*
				 * TIM1: 32-bits field
				 * TIM2 to TIM5: 16-bits field
				 */
				if(userTIMx == my_TIM1){
					bitWidth = 32;
				}
				else bitWidth = 16; //TIM2 to TIM5
				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM2_OR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM2_OR)){
				reg = &TIMx_p -> TIM2_OR;
				bitWidth = 2; //ITR1_RMP [1:0] at bit 10
				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM5_OR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM5_OR)){
				reg = &TIMx_p -> TIM5_OR;
				bitWidth = 2; //TI4_RMP at bit 6
				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		case TIM11_OR:
			if(isValidTimerBit(bitPosition, userTIMx, TIM11_OR)){
				reg = &TIMx_p -> TIM11_OR;
				bitWidth = 2; //TI1_RMP at bit 0
				return readBits(reg, bitPosition, bitWidth);
			} else return ERROR_FLAG;
			break;


		default: return ERROR_FLAG; //Invalid mode
	}
}





















