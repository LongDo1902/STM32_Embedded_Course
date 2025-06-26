/*
 * exti.c
 *
 *  Created on: Jun 26, 2025
 *      Author: dobao
 */

#include "exti.h"

void writeNVIC(IQRn_Pos_t iqrPosition, volatile uint32_t* nvicAddr, FlagStatus value){
	if(iqrPosition > 31) return; //Invalid bit access

	if(value == SET){
		*nvicAddr |= (SET << iqrPosition);
	}
	else *nvicAddr &= ~(1U << iqrPosition);
}
