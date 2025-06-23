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
		 * TIMx_CR1:
		 * 		TIM1 to TIM5: valid bits are [0:9]
		 * 		TIM9 to TIM11: valid bits are [0:9 excluding bits 4 to 6 (Reserved)
		 */
		case TIM_CR1:
			if((userTIMx >= my_TIM1 && userTIMx <= my_TIM5 && bitPosition <= 9) ||
			   (userTIMx >= my_TIM9 && bitPosition <= 9 && !(bitPosition >= 4 && bitPosition <= 6))){
				reg = &TIMx_p -> TIM_CR1; //Storing addr of that reg field in reg
			}
			else return;
		break;


		/*
		 * TIMx_CR2:
		 * 		Only available in TIM1 to TIM5
		 * 		Bits 1 and 15 are reserved
		 */
		case TIM_CR2:
			if(userTIMx >= my_TIM1 && userTIMx <= my_TIM5 && bitPosition != 1 && bitPosition != 15){
				reg = &TIMx_p -> TIM_CR2;
			}
			else return;
		break;


		/*
		 * TIMx_SMCR:
		 * 		Available in TIM1 to TIM5 and TIM9
		 * 		Bit 3 is reserved for all
		 * 		TIM9 has additional reserved bits [8:15]
		 */
		case TIM_SMCR:
			if((userTIMx >= my_TIM1 && userTIMx <= my_TIM5 && bitPosition != 3) ||
				(userTIMx == my_TIM9 && bitPosition != 3 && bitPosition <= 7)){
				reg = &TIMx_p -> TIM_SMCR;
			}
			else return;
		break;


		/*
		 * TIMx_DIER:
		 * 		TIM1: all bits valid except bit 15
		 * 		TIM2 to TIM5: bits 5, 7, 13, 15 are reserved
		 * 		TIM9: valid bits are 0, 1, 2, and 6
		 * 		TIM10/11: only bits 0 and 1 are valid
		 */
		case TIM_DIER:
			if((userTIMx == my_TIM1 && bitPosition != 15) ||
			   ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && bitPosition != 5 && bitPosition != 7 && bitPosition != 13 && bitPosition != 15) ||
			   (userTIMx == my_TIM9 && (bitPosition <= 2 || bitPosition == 6)) ||
			   ((userTIMx == my_TIM10 || userTIMx == my_TIM11) && bitPosition <= 1)){
				reg = &TIMx_p -> TIM_DIER;
			}
			else return;
		break;


		/*
		 * TIMx_SR
		 * 		TIM1: bit 8 and bits > 12 are reserved
		 * 		TIM2 to TIM5: valid bits <= 12 except 5, 7, 8
		 * 		TIM9  to TIM11: valid bits <= 10 except 3, 4, 5, 7, 8
		 */
		case TIM_SR:
			if((userTIMx == my_TIM1 && bitPosition <= 12 && bitPosition != 8) ||
			   ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && bitPosition <= 12 && (bitPosition != 5 && bitPosition != 7 && bitPosition != 8)) ||
			   ((userTIMx >= my_TIM9 && userTIMx <= my_TIM11) && bitPosition <= 10 && (!(bitPosition >= 3 && bitPosition <= 5) && bitPosition != 7 && bitPosition != 8))){
				reg = &TIMx_p -> TIM_SR;
			}
			else return;
		break;


		/*
		 * TIMx_EGR
		 * 		TIM1: bits [0:7] are valid
		 * 		TIM2 to TIM5: bits[0:6] valid except bit 5
		 * 		TIM10/11: only bits 0 and 1 are valid
		 */
		case TIM_EGR:
			if((userTIMx == my_TIM1 && bitPosition <= 7) ||
			  ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && bitPosition <= 6 && bitPosition != 5) ||
			  ((userTIMx == my_TIM10 || userTIMx == my_TIM11) && bitPosition <= 1)){
				reg = &TIMx_p -> TIM_EGR;
			}
			else return;
		break;


		/*
		 * TIMx_CCER
		 * 		TIM1: all bits valid except 14
		 * 		TIM2 to TIM5: bits 2, 5, 6, 10, 14 are reserved
		 * 		TIM9: valid bits <= 7 excluding 2, 6
		 * 		TIM10/11: valid bits <= 3 excluding 2
		 */
		case TIM_CCER:
			if((userTIMx == my_TIM1 && bitPosition != 14) ||
			   ((userTIMx >= my_TIM2 && userTIMx <= my_TIM5) && bitPosition != 14 && bitPosition != 10 && bitPosition != 6 && bitPosition != 2) ||
			   ((userTIMx == my_TIM9 && bitPosition <= 7 && bitPosition != 6 && bitPosition != 2) ||
			   ((userTIMx == my_TIM10 || userTIMx == my_TIM11) && bitPosition <= 3 && bitPosition != 2))){
				reg = &TIMx_p -> TIM_CCER;
			}

			else return;
		break;


		/*
		 * TIMx_CNT, TIMx_PSC, TIMx_ARR
		 * 		Alltimers have these registers
		 * 		No reserved bits, canbe written freely
		 */
		case TIM_CNT:	reg = &TIMx_p -> TIM_CNT; break; //No reserved bits
		case TIM_PSC:	reg = &TIMx_p -> TIM_PSC; break; //No reserved bits
		case TIM_ARR:	reg = &TIMx_p -> TIM_ARR; break; //No reserved bits


		/*
		 * TIMx_RCR
		 * 		Only available in TIM1
		 * 		Bits [0:7] are valid
		 */
		case TIM_RCR: //Only available in TIMER1
			if(userTIMx == my_TIM1 && bitPosition <= 7){
				reg = &TIMx_p -> TIM_RCR;
			}
			else return;
		break;


		/*
		 * TIMx_CCR1 is available on all timers
		 */
		case TIM_CCR1: reg = &TIMx_p -> TIM_CCR1; break; //All TIMERs have CCR1


		/*
		 * TIMx_CCR2:
		 * 		Not available on TIM10 and TIM11
		 */
		case TIM_CCR2:
			if(!(userTIMx == my_TIM10 || userTIMx == my_TIM11)){ //Only TIMER1/2/3/4/5/9 have CCR2
				reg = &TIMx_p -> TIM_CCR2;
			}
			else return;
		break;


		/*
		 * TIMx_CCR3:
		 * 		Only available in TIM1 to TIM5
		 */
		case TIM_CCR3:
			if(!(userTIMx >= my_TIM9)){ //Only TIMER1/2/3/4/5 have CCR3
				reg = &TIMx_p -> TIM_CCR3;
			}
			else return;
		break;


		/*
		 * TIMx_CCR4:
		 *  	Only available in TIM1 to TIM5
		 */
		case TIM_CCR4:
			if(!(userTIMx >= my_TIM9)){ //Only TIMER1/2/3/4/5 have CCR4
				reg = &TIMx_p -> TIM_CCR4;
			}
			else return;
		break;


		/*
		 * TIMx_BDTR:
		 * 		Only available in TIM1
		 */
		case TIM_BDTR:
			if(userTIMx == my_TIM1){
				reg = &TIMx_p -> TIM_BDTR;
			}
			else return;
		break;


		/*
		 * TIMx_DCR:
		 * 		Only available in TIM1 to TIM5
		 * 		All have reserved bits in [5:7] and [13:15]
		 */
		case TIM_DCR:
			if((userTIMx >= my_TIM1 && userTIMx <= my_TIM5) && !(bitPosition >= 5 && bitPosition <= 7) && !(bitPosition >= 13 && bitPosition <= 15)){
				reg = &TIMx_p -> TIM_DCR;
			}
			else return;
		break;


		/*
		 * TIMx_DMAR:
		 * 		Only available in TIM1 to TIM5
		 * 		Have no reserved bits
		 */
		case TIM_DMAR:
			if(userTIMx >= my_TIM1 && userTIMx <= my_TIM5){
				reg = &TIMx_p -> TIM_DMAR;
			} else return;
		break;


		/*
		 * TIMx_OR:
		 * 		Only available in TIM2 and TIM5
		 * 		TIM2 has valid bits at 10 and 11
		 * 		TIM5 has valid bits at 6 and 7
		 */
		case TIM_OR:
			if((userTIMx == my_TIM2 && bitPosition == 10 && bitPosition == 11) ||
			   (userTIMx == my_TIM5 && bitPosition == 6 && bitPosition == 7)){
				reg = &TIMx_p -> TIM_OR;
			} else return;
		break;

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
		 *TIMER2
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

