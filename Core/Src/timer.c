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
 * @brief	Helper function to read...
 */
uint32_t readTimer (uint8_t bitPosition, TIM_Name_t userTIMx, TIM_Mode_t mode){
	const uint16_t ERROR_FLAG = 0xFFFF;

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
		/*
		 * TIMER1
		 */
		case my_TIM1:
			TIMx_p = TIM1_REG;

			switch(mode){
				case TIM_CR1:
					if(bitPosition > 9) return ERROR_FLAG; //Block reserved bits

					reg = &TIMx_p -> TIM_CR1;
					if(bitPosition == 5 || bitPosition == 8){
						//CMS[1:0] at 5: bit 5 to 6 -> 2 bits
						//CKD[1:0] at 8: bit 8 to 9 -> 2 bits
						return ((*reg >> bitPosition) & 0b11);
					}
					else return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_CR2:
					if(bitPosition == 1 || bitPosition == 15) return ERROR_FLAG; //Block reserved bits

					reg = &TIMx_p -> TIM_CR2;
					if(bitPosition == 4){
						//MMS[0:2] at bit 4: bit 4 to 6 -> 3 bits
						return ((*reg >> bitPosition) & 0b111);
					} else return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_SMCR:
					if(bitPosition == 3) return ERROR_FLAG; //Block reserved bits

					reg = &TIMx_p -> TIM_SMCR;
					if(bitPosition == 0 || bitPosition == 4){
						//SMS[2:0] at 0: bit 0 to 2 -> 3 bits
						//TS[2:0] at 4: bit 4 to 6 -> 3 bits
						return ((*reg >> bitPosition) & 0b111);
					}
					else if(bitPosition == 8){
						//ETF[3:0] at 8: bit 8 to 11 -> 4 bits
						return ((*reg >> bitPosition) & 0b1111);
					}
					else if(bitPosition == 12){
						//ETPS[1:0] at 12: bit 12 to 13 -> 2 bits
						return ((*reg >> bitPosition) & 0b11);
					}
					else return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_DIER:
					if(bitPosition == 15) return ERROR_FLAG; //Blocks reserved bits

					reg = &TIMx_p -> TIM_DIER;
					return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_SR:
					if(bitPosition > 12 || bitPosition == 8) return ERROR_FLAG; //Blocks reserved bits

					reg = &TIMx_p -> TIM_SR;
					return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_EGR:
					if(bitPosition > 7) return ERROR_FLAG; //Block reserved bits

					reg = &TIMx_p -> TIM_EGR;
					return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_CCER:
					if(bitPosition == 14) return ERROR_FLAG; //Block reserved bits

					reg = &TIMx_p -> TIM_CCER;
					return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_CNT:
					reg = &TIMx_p -> TIM_CNT;
					return (*reg & 0xFFFF);
				break;


				case TIM_PSC:
					reg = &TIMx_p -> TIM_PSC;
					return (*reg & 0xFFFF);
				break;


				case TIM_ARR:
					reg = &TIMx_p -> TIM_ARR;
					return (*reg & 0xFFFF);
				break;


				case TIM_RCR:
					if(bitPosition > 7) return ERROR_FLAG; //Block reserved bits

					reg = &TIMx_p -> TIM_RCR;
					return (*reg & 0xFF);
				break;


				case TIM_CCR1:
					reg = &TIMx_p -> TIM_CCR1;
					return (*reg & 0xFFFF);
				break;


				case TIM_CCR2:
					reg = &TIMx_p -> TIM_CCR2;
					return (*reg & 0xFFFF);
				break;


				case TIM_CCR3:
					reg = &TIMx_p -> TIM_CCR3;
					return (*reg & 0xFFFF);
				break;


				case TIM_CCR4:
					reg = &TIMx_p -> TIM_CCR4;
					return (*reg & 0xFFFF);
				break;


				case TIM_BDTR:
					reg = &TIMx_p -> TIM_BDTR;
					if(bitPosition == 0){
						//DTG[7:0] at bit 0: bit 0 to 7 -> 1 byte
						return (*reg & 0xFF);
					}
					else if(bitPosition == 8){
						//LOCK[1:0] at bit 8: bit 8 to 9 -> 2 bits
						return ((*reg >> bitPosition) & 0b11);
					}
					else return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_DCR:
					if((bitPosition >= 5 && bitPosition <= 7) || (bitPosition >= 13 && bitPosition <= 15)) return ERROR_FLAG;

					reg = &TIMx_p -> TIM_DCR;
					if(bitPosition == 0 || bitPosition == 8){
					//DBA[4:0] at bit 0: bit 0 to 4 -> 5 bits
					//DBL[4:0] at bit 8: bit 8 to 12 -> 5 bits
					return ((*reg >> bitPosition) & 0b11111);
					}
					else return ERROR_FLAG;
				break;


				case TIM_DMAR:
					reg = &TIMx_p -> TIM_DMAR;
					return (*reg & 0xFFFFFFFF);
				break;

				default: return ERROR_FLAG;
			}
		break;


		/*
		 *TIMER2 (32-bits timer)
		 */
		case my_TIM2:
			TIMx_p = TIM2_REG;
			switch(mode){
				case TIM_CR1:
					if(bitPosition > 9) return ERROR_FLAG; //Block reserved bits

					reg = &TIMx_p -> TIM_CR1;
					if(bitPosition == 5 || bitPosition == 8){
						//CMS[1:0] at 5: bit 5 to 6 -> 2 bits
						//CKD[1:0] at 8: bit 8 to 9 -> 2 bits
						return ((*reg >> bitPosition) & 0b11);
					}
					else return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_CR2:
					if((bitPosition >= 0 && bitPosition <= 2) || (bitPosition > 7)) return ERROR_FLAG;
					reg = &TIMx_p -> TIM_CR2;
					if(bitPosition == 4){
						//MMS[0:2] at bit 4: bit 4 to 6 -> 3 bits
						return ((*reg >> bitPosition) & 0b111);
					} else return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_SMCR:
					if(bitPosition == 3) return ERROR_FLAG; //Block reserved bits

					reg = &TIMx_p -> TIM_SMCR;
					if(bitPosition == 0 || bitPosition == 4){
						//SMS[2:0] at 0: bit 0 to 2 -> 3 bits
						//TS[2:0] at 4: bit 4 to 6 -> 3 bits
						return ((*reg >> bitPosition) & 0b111);
					}
					else if(bitPosition == 8){
						//ETF[3:0] at 8: bit 8 to 11 -> 4 bits
						return ((*reg >> bitPosition) & 0b1111);
					}
					else if(bitPosition == 12){
						//ETPS[1:0] at 12: bit 12 to 13 -> 2 bits
						return ((*reg >> bitPosition) & 0b11);
					}
					else return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_DIER:
					if(bitPosition == 15 || bitPosition == 13 || bitPosition == 7 || bitPosition == 5) return ERROR_FLAG; //Blocks reserved bits

					reg = &TIMx_p -> TIM_DIER;
					return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_SR:
					if(bitPosition > 12 || bitPosition == 8 || bitPosition == 7 || bitPosition == 5) return ERROR_FLAG; //Blocks reserved bits

					reg = &TIMx_p -> TIM_SR;
					return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_EGR:
					if(bitPosition > 7 || bitPosition == 5) return ERROR_FLAG; //Block reserved bits

					reg = &TIMx_p -> TIM_EGR;
					return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_CCER:
					if(bitPosition == 14 || bitPosition == 10 || bitPosition == 6 || bitPosition == 2) return ERROR_FLAG; //Block reserved bits

					reg = &TIMx_p -> TIM_CCER;
					return ((*reg >> bitPosition) & 0b1);
				break;


				case TIM_CNT:
					reg = &TIMx_p -> TIM_CNT;
					return (*reg & 0xFFFFFFFF); //TIM2 has total 32bits
				break;


				case TIM_PSC:
					reg = &TIMx_p -> TIM_PSC;
					return (*reg & 0xFFFF);
				break;


				case TIM_ARR:
					reg = &TIMx_p -> TIM_ARR;
					return (*reg & 0xFFFF);
				break;


				case TIM_CCR1:
					reg = &TIMx_p -> TIM_CCR1;
					return (*reg & 0xFFFF);
				break;


				case TIM_CCR2:
					reg = &TIMx_p -> TIM_CCR2;
					return (*reg & 0xFFFF);
				break;


				case TIM_CCR3:
					reg = &TIMx_p -> TIM_CCR3;
					return (*reg & 0xFFFF);
				break;


				case TIM_CCR4:
					reg = &TIMx_p -> TIM_CCR4;
					return (*reg & 0xFFFF);
				break;


				case TIM_DCR:
					if((bitPosition >= 5 && bitPosition <= 7) || (bitPosition > 12)) return ERROR_FLAG;

					reg = &TIMx_p -> TIM_DCR;
					if(bitPosition == 0 || bitPosition == 8){
					//DBA[4:0] at bit 0: bit 0 to 4 -> 5 bits
					//DBL[4:0] at bit 8: bit 8 to 12 -> 5 bits
					return ((*reg >> bitPosition) & 0b11111);
					}
					else return ERROR_FLAG;
				break;


				case TIM_DMAR:
					reg = &TIMx_p -> TIM_DMAR;
					return (*reg & 0xFFFFFFFF);
				break;

				default: return ERROR_FLAG;
			}
		break;


		case my_TIM3: TIMx_p = TIM3_REG; break;
		case my_TIM4: TIMx_p = TIM4_REG; break;
		case my_TIM5: TIMx_p = TIM5_REG; break;



		case my_TIM9: TIMx_p = TIM9_REG; break;
		case my_TIM10: TIMx_p = TIM10_REG; break;
		case my_TIM11: TIMx_p = TIM11_REG; break;
		default: return ERROR_FLAG;
	}
}
