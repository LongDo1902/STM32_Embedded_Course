/*
 * exti.h
 *
 *  Created on: Jun 26, 2025
 *      Author: dobao
 */

#ifndef INC_EXTI_H_
#define INC_EXTI_H_

#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "registerAddress.h"


/*
 * Collections of EXTI Offset Register Name
 */
typedef enum{IMR, EMR, RTSR, FTSR, SWIER, PR}EXTI_Mode_t;


/*
 * Vector Table Positions
 */
typedef enum{
	//Group 1
	WWDG_user = 0,
	EXTI16_PVD,
	EXTI21_TAMP_STAMP,
	EXTI22_RTC_WKUP,
	FLASH_user,
	RCC_user,
	EXTI0 = 6,
	EXTI1,
	EXTI2,
	EXTI3,
	EXTI4,
	DMA1_S0,
	DMA1_S1,
	DMA1_S2,
	DMA1_S3,
	DMA1_S4,
	DMA1_S5,
	DMA1_S6,
	ADC_user,

	//Group 2
	EXTI9_5 = 23,
	TIM1_BRK_TIM9,
	TIM1_UP_TIM10,
	TIM1_TGR_COM_TIM11,
	TIM1_CC,
	TIM2_user,
	TIM3_user,
	TIM4_user,
	I2C1_EV,
	I2C1_ER,
	I2C2_EV,
	I2C2_ER,
	SPI1_user,
	SPI2_user,
	UART1,
	UART2,

	//Group 3
	EXTI15_10 = 40,
	EXTI17_RTC_ALARM,
	EXTI18_OTG_FS_WKUP,
	DMA1_S7 = 47,
	SDIO_user = 49,
	TIM5_user,
	SPI3_user,
	DMA2_S0	= 56,
	DMA2_S1,
	DMA2_S2,
	DMA2_S3,
	DMA2_S4,
	OTG_FS = 67,
	DMA2_S5,
	DMA2_S6,
	DMA2_S7,
	UART6,
	I2C3_EV,
	I2C3_ER,
	FPU_user = 81,
	SPI4_user = 84,
	SPI5_user = 85
}IQRn_Pos_t;

typedef enum{
	NVIC_ISER, 	//Interrupt Set-Enable Reg
	NVIC_ICER, 	//Intertupt Clear-Enable Reg
	NVIC_ISPR, 	//Interrupt Set-Pending Reg
	NVIC_ICPR,	//Interrupt Clear-Pending Reg
	NVIC_IABR,	//Interrupt Active Bit Reg
	NVIC_IPR	//Interrupt Priority Reg
}NVIC_Name_t;


/*
 * FUNCTIONS DECLARATION
 */
void NVIC_enableIRQ(uint16_t irqNumber, NVIC_Name_t nvicName);
void NVIC_disableIRQ(uint32_t irqNumber, NVIC_Name_t nvicName);
void NVIC_writeIPR(uint16_t irqNumber, uint8_t priority);

#endif /* INC_EXTI_H_ */
