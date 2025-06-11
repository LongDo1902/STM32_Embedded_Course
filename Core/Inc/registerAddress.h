/*
 * RES_BASE_ADDR.h
 *
 *  Created on: Mar 28, 2025
 *      Author: dobao
 */

#ifndef INC_RES_ADDR_H_
#define INC_RES_ADDR_H_

#include<stdio.h>
#include <stdint.h>

/*
 * GPIOA to GPIOH ports base addresses
 */
#define GPIOA_BASE_ADDR 0x40020000UL //UL: Unsigned Long
#define GPIOB_BASE_ADDR 0x40020400UL
#define GPIOC_BASE_ADDR 0x40020800UL
#define GPIOD_BASE_ADDR 0x40020C00UL
#define GPIOE_BASE_ADDR 0x40021000UL
#define GPIOH_BASE_ADDR 0x40021C00UL

/*
 * EXTI base address
 */
#define EXTI_BASE_ADDR 0x40013C00UL

/*
 * UART base addresses
 */
#define UART1_BASE_ADDR 0x40011000UL
#define UART2_BASE_ADDR 0x40004400UL
#define UART6_BASE_ADDR 0x40011400UL

/*
 * SPI base addresses
 */
#define SPI1_BASE_ADDR 0x40013000UL
#define SPI2_BASE_ADDR 0x40003800UL
#define SPI3_BASE_ADDR 0x40003C00UL
#define SPI4_BASE_ADDR 0x40013400UL
#define SPI5_BASE_ADDR 0x40015000UL

/*
 * I2C base addresses
 */
#define I2C1_BASE_ADDR 0x40005400UL
#define I2C2_BASE_ADDR 0x40005800UL
#define I2C3_BASE_ADDR 0x40005C00UL

/*
 * TIMER base addresses
 */
#define TIM1_BASE_ADDR 	0x40010000UL
#define TIM2_BASE_ADDR 	0x40000000UL
#define TIM3_BASE_ADDR 	0x40000400UL
#define TIM4_BASE_ADDR 	0x40000800UL
#define TIM5_BASE_ADDR 	0x40000C00UL
#define TIM9_BASE_ADDR 	0x40014000UL
#define TIM10_BASE_ADDR	0x40014400UL
#define TIM11_BASE_ADDR	0x40014800UL

/*
 * FLASH INTERFACE REG ADDR
 */
#define FLASH_INTF_REG_ADDR 0x40023800UL
////////////END OF BASE ADDRESSES////////////



/*
 * GPIO Registers Offsets
 */
typedef struct{
	volatile uint32_t MODER; 	//0x00 (Mode Reg)
	volatile uint32_t OTYPER; 	//0x04 (Output Type Reg)
	volatile uint32_t OSPEEDR; 	//0x08 (Output Speed Reg)
	volatile uint32_t PUPDR;	//0x0C (Pull-up/pull-down Reg)
	volatile uint32_t IDR;		//0x10 (Input Data Reg)
	volatile uint32_t ODR;		//0x14 (Output Data Reg)
	volatile uint32_t BSRR;		//0x18 (Bit Set/Reset Reg)
	volatile uint32_t LCKR;		//0x1C (GPIO Port Configuration Lock Register)
	volatile uint32_t AFRL;		//0x20 (Alternate Function Low Reg)
	volatile uint32_t AFRH;		//0x24 (Alternate Function High Reg)
}GPIO_Register_Offset_t;

/*
 * EXTI Registers Offsets
 */
typedef struct{
	volatile uint32_t IMR; 		//0x00 (Interrupt Mask Reg)
	volatile uint32_t EMR; 		//0x04 (Event Mask Reg)
	volatile uint32_t RTSR;		//0x08 (Rising Trigger Selection Reg)
	volatile uint32_t FTSR;		//0x0C (Falling Trigger Selection Reg)
	volatile uint32_t SWIER; 	//0x10 (Software Interrupt Event Reg)
	volatile uint32_t PR;		//0x14 (Pending Reg)
}EXTI_Register_Offset_t;

/*
 * UART Register Offsets
 */
typedef struct{
	volatile uint32_t SR;		//0x00 (Status Reg)
	volatile uint32_t DR;		//0x04 (Data Reg)
	volatile uint32_t BRR;		//0x08 (Baud Rate Reg)
	volatile uint32_t CR1;		//0x0C (Control Reg 1)
	volatile uint32_t CR2;		//0x10 (Control Reg 2)
	volatile uint32_t CR3;		//0x14 (Control Reg 3)
	volatile uint32_t GTPR;		//0x18 (Guard Time and Prescaler Reg)
}UART_Register_Offset_t;

/*
 * I2C Register Offsets
 */
typedef struct{
	volatile uint32_t CR1;		//0x00 (Control Reg 1)
	volatile uint32_t CR2;		//0x04 (Control Reg 2)
	volatile uint32_t OAR1;		//0x08 (Own Addr Reg 1)
	volatile uint32_t OAR2;		//0x0C (Own Addr Reg 2)
	volatile uint32_t DR;		//0x10 (Data Reg)
	volatile uint32_t SR1;		//0x14 (Status Reg 1)
	volatile uint32_t SR2;		//0x18 (Status Reg 2)
	volatile uint32_t CCR;		//0x1C (Clock Control Reg)
	volatile uint32_t TRISE;	//0x20 (TRISE Reg)
	volatile uint32_t FLTR;		//0x24 (FLTR Reg)
}I2C_Register_Offset_t;

/*
 * SPI Register Offsets
 */
typedef struct{
	volatile uint32_t SPI_CR1;		//0x00 (Control Reg 1)
	volatile uint32_t SPI_CR2;		//0x04 (Control Reg 2)
	volatile uint32_t SPI_SR;		//0x08 (Status Reg)
	volatile uint32_t SPI_DR;		//0x0C (Data Reg)
	volatile uint32_t SPI_CRC;		//0x10 (CRC Polynomial Reg)
	volatile uint32_t SPI_RXCRCR;	//0x14 (RX CRC Reg)
	volatile uint32_t SPI_TXCRCR;	//0x18 (TX CRC Reg)
	volatile uint32_t SPI_I2SCFGR;	//0x1C (I2S Config Reg)
	volatile uint32_t SPI_I2SPR;	//0x20 (I2S Prescaler Reg)
}SPI_Register_Offset_t;

/*
 * FLASH Interface Register Offset
 */
typedef struct {
	volatile uint32_t FLASH_ACR;		//0x00 (Flash Access Control Reg)
	volatile uint32_t FLASH_KEYR;		//0x04 (Flash Key Reg)
	volatile uint32_t FLASH_OPTKEYR;	//0x08 (Flash Option Key Reg)
	volatile uint32_t FLASH_SR;			//0x0C (Flash Status Reg)
	volatile uint32_t FLASH_CR;			//0x10 (Flash Control Reg)
	volatile uint32_t FLASH_OPTCR;		//0x14 (Flash Option Control Reg)
}Flash_IntF_Register_Offset_t;


////////////END OF REGISTER OFFSET STRUCTS////////////



/*
 * GPIOs Reg Pointers
 */
#define GPIOA_REG ((GPIO_Register_Offset_t*)GPIOA_BASE_ADDR)
#define GPIOB_REG ((GPIO_Register_Offset_t*)GPIOB_BASE_ADDR)
#define GPIOC_REG ((GPIO_Register_Offset_t*)GPIOC_BASE_ADDR)
#define GPIOD_REG ((GPIO_Register_Offset_t*)GPIOD_BASE_ADDR)
#define GPIOE_REG ((GPIO_Register_Offset_t*)GPIOE_BASE_ADDR)
#define GPIOH_REG ((GPIO_Register_Offset_t*)GPIOH_BASE_ADDR)

/*
 * EXTI Reg Pointers
 */
#define EXTI_REG ((EXTI_Register_Offset_t*)EXTI_BASE_ADDR)

/*
 * UART Reg Pointers
 */
#define UART1_REG ((UART_Register_Offset_t*)UART1_BASE_ADDR)
#define UART2_REG ((UART_Register_Offset_t*)UART2_BASE_ADDR)
#define UART6_REG ((UART_Register_Offset_t*)UART6_BASE_ADDR)

/*
 * SPI Reg Pointers
 */
#define SPI1_REG ((SPI_Register_Offset_t*)SPI1_BASE_ADDR)
#define SPI2_REG ((SPI_Register_Offset_t*)SPI2_BASE_ADDR)
#define SPI3_REG ((SPI_Register_Offset_t*)SPI3_BASE_ADDR)
#define SPI4_REG ((SPI_Register_Offset_t*)SPI4_BASE_ADDR)
#define SPI5_REG ((SPI_Register_Offset_t*)SPI5_BASE_ADDR)

/*
 * Flash Reg Pointers
 */
#define FLASH_REG ((Flash_IntF_Register_Offset_t*) FLASH_INTF_REG_ADDR)
////////////END OF REGISTER POINTERS////////////



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
}IQRn_User_t;
////////////END OF ENUM////////////

/*
 * Custom symbolic addresses for EXTI IRQ vector labels
 */
#define EXTI0_ADDR 0x58
#define EXTI1_ADDR 0x5C
#define EXTI2_ADDR 0x60
#define EXTI3_ADDR 0x64

/*
 * NVIC - Interrupt Set-Enable Reg (Cortex-M4 Ref Manual)
 */
#define NVIC_ISER0 ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1 ((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2 ((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3 ((volatile uint32_t*)0xE000E10C)
#define NVIC_ISER4 ((volatile uint32_t*)0xE000E110)
#define NVIC_ISER5 ((volatile uint32_t*)0xE000E114)
#define NVIC_ISER6 ((volatile uint32_t*)0xE000E118)
#define NVIC_ISER7 ((volatile uint32_t*)0xE000E11C)

/*
 * VTOR (Vector Table Offset Reg) (Cortex-M4 ref manual)
 */
#define VTO_REG ((volatile uint32_t*)(0xE000ED08))
///////////////END OF ADDRESSES DEFINES///////////////



#endif /* INC_RES_ADDR_H_ */
