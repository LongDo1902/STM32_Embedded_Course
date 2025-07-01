/*
 * RES_BASE_ADDR.h
 *
 *  Created on: Mar 28, 2025
 *      Author: dobao
 */

#ifndef INC_RES_ADDR_H_
#define INC_RES_ADDR_H_

#include <stdio.h>
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
 * EXTI base address
 */
#define EXTI_BASE_ADDR 0x40013C00UL

/*
 * NVIC Registers (Cortex-M4 Ref Manual)
 */
#define NVIC_BASE_ADDR 0xE000E100UL

/*
 * VTOR (Vector Table Offset Reg) (Cortex-M4 ref manual)
 */
#define VTOR_BASE_ADDR 0xE000ED08UL

/*
 * Custom symbolic addresses for EXTI IRQ vector labels
 */
#define EXTI0_ADDR 0x58
#define EXTI1_ADDR 0x5C
#define EXTI2_ADDR 0x60
#define EXTI3_ADDR 0x64

/*
 * RCC Base Address
 */
#define RCC_BASE_ADDR 0x40023800UL


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
	volatile uint32_t ODR ;		//0x14 (Output Data Reg)
	volatile uint32_t BSRR; 	//0x18 (Bit Set/Reset Reg)
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
 * TIMER 1 2 3 4 5, 9 10 11 Register Offsets
 */
typedef struct{
	volatile uint32_t TIM_CR1;	//0x00 (Control Reg 1)
	volatile uint32_t TIM_CR2;	//0x04 (Control Reg 2)
	volatile uint32_t TIM_SMCR;	//0x08 (Slave Mode Control Reg)
	volatile uint32_t TIM_DIER;	//0x0C (DMA/Interrupt Enable Reg)
	volatile uint32_t TIM_SR;	//0x10 (Status Reg)

	volatile uint32_t TIM_EGR;		//0x14 (Event Generation Reg)
	volatile uint32_t TIM_CCMR1;	//0x18 (Capture/Compare Mode Reg 1)
	volatile uint32_t TIM_CCMR2;	//0x1C (Capture/Compare Mode Reg 2)
	volatile uint32_t TIM_CCER;		//0x20 (Capture/Compare Enable Reg)
	volatile uint32_t TIM_CNT;		//0x24 (Counter)

	volatile uint32_t TIM_PSC;		//0x28 (Prescaler)
	volatile uint32_t TIM_ARR;		//0x2C (Auto-reload Reg)
	volatile uint32_t TIM_RCR;		//0x30 (Repetition Counter Reg) (Only TIM1)

	volatile uint32_t TIM_CCR1;		//0x34 (Capture/Compare Reg 1)
	volatile uint32_t TIM_CCR2;		//0x38 (Capture/Compare Reg 2)
	volatile uint32_t TIM_CCR3;		//0x3C (Capture/Compare Reg 3)
	volatile uint32_t TIM_CCR4;		//0x40 (Capture/Compare Reg 4)

	volatile uint32_t TIM_BDTR;		//Only TIM1 (Break & Dead-time Reg)

	volatile uint32_t TIM_DCR;		//0x48 (DMA Control Reg)
	volatile uint32_t TIM_DMAR;		//0x4C (DMA Address for Full Transfer)

	union{	//0x50 (Option Reg) (TIM2 and TIM5 and TIM11)
		volatile uint32_t TIM2_OR;
		volatile uint32_t TIM5_OR;
		volatile uint32_t TIM11_OR;
	};
}TIM_Register_Offset_t;

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

/*
 *
 */
typedef struct{
	volatile uint32_t _ISER[8];		//0xE000E100 (Interrupt Set Enable Reg)
	uint32_t RESERVED0[24]; 	//Reserved gap (0xE000E120 to 0xE000E17F)
	volatile uint32_t _ICER[8]; 	//0xE000E180 (Interrupt Clear-Enable Reg)
	uint32_t RESERVED1[24];
	volatile uint32_t _ISPR[8]; 	//0xE000E200 (Interrupt Set Pending Reg)
	uint32_t RESERVED2[24];
	volatile uint32_t _ICPR[8]; 	//0xE000E280 (Interrupt Clear-Pending Reg)
	uint32_t RESERVED3[24];
	volatile uint32_t _IABR[8]; 	//0xE000E300 (Interrupt Active Bit Reg)
	uint32_t RESERVED4[56];
	volatile uint8_t _IPR[240]; 	//0xE000E400 (Interrupt Priority Reg 1 byte each)
}NVIC_t;

/*
 * List of RCC Register Offsets
 */
typedef struct{
	volatile uint32_t RCC_RC;			//0x00 (RCC Clock Control Reg)
	volatile uint32_t RCC_PLL_CFGR;		//0x04 (RCC PLL Config Reg)
	volatile uint32_t RCC_CFGR;			//0x08 (RCC Clock Config Reg)
	volatile uint32_t RCC_CIR;			//0x0C (RCC Clock Interrupt Reg)

	volatile uint32_t RCC_AHB1_RSTR;	//0x10 (RCC AHB1 Reset Reg)
	volatile uint32_t RCC_AHB2_RSTR;	//0x14 (RCC AHB2 Reset Reg)

	volatile uint32_t RESERVED0;
	volatile uint32_t RESERVED1;

	volatile uint32_t RCC_APB1_RSTR;	//0x20 (RCC APB1 Reset Reg)
	volatile uint32_t RCC_APB2_RSTR;	//0x24 (RCC APB2 Reset Reg)

	volatile uint32_t RESERVED2;
	volatile uint32_t RESERVED3;

	volatile uint32_t RCC_AHB1_ENR;		//0x30 (RCC AHB1 Clock Enable)
	volatile uint32_t RCC_AHB2_ENR;		//0x34 (RCC AHB2 Clock Enable)

	volatile uint32_t RESERVED4;
	volatile uint32_t RESERVED5;

	volatile uint32_t RCC_APB1_ENR;		//0x40 (RCC APB1 Clock Enable)
	volatile uint32_t RCC_APB2_ENR;		//0x44 (RCC APB2 Clock Enable)

	volatile uint32_t RESERVED6;
	volatile uint32_t RESERVED7;

	volatile uint32_t RCC_AHB1_LP_ENR;	//0x50 (RCC AHB1 Low Power Mode Enable)
	volatile uint32_t RCC_AHB2_LP_ENR;	//0x54 (RCC AHB2 Low Power Mode Enable)

	volatile uint32_t RESERVED8;
	volatile uint32_t RESERVED9;

	volatile uint32_t RCC_APB1_LP_ENR;	//0x60 (RCC APB1 Low Power Mode Enable)
	volatile uint32_t RCC_APB2_LP_ENR;	//0x64 (RCC APB2 Low Power Mode Enable)

	volatile uint32_t RESERVED10;
	volatile uint32_t RESERVED11;

	volatile uint32_t RCC_BDCR;			//0x70 (RCC Backup Domain Control Reg)
	volatile uint32_t RCC_CSR;			//0x74 (RCC Clock Control & Status Reg)

	volatile uint32_t RESERVED12;
	volatile uint32_t RESERVED13;

	volatile uint32_t RCC_SSCGR;		//0x80 (RCC Spread Spectrum Clock Generation Reg)
	volatile uint32_t RCC_PLL_I2S_CFGR;	//0x84 (RCC PLLI2S Config Reg)

	volatile uint32_t RESERVED14;

	volatile uint32_t RCC_DCK_CFGR;		//0x8C (RCC Dedicated Clocks Config Reg)
}RCC_Register_Offset_t;

////////////END OF REGISTER OFFSET STRUCTS////////////

/*
 * GPIOs Reg Pointers
 */
#define GPIOA_REG ((volatile GPIO_Register_Offset_t*)GPIOA_BASE_ADDR)
#define GPIOB_REG ((volatile GPIO_Register_Offset_t*)GPIOB_BASE_ADDR)
#define GPIOC_REG ((volatile GPIO_Register_Offset_t*)GPIOC_BASE_ADDR)
#define GPIOD_REG ((volatile GPIO_Register_Offset_t*)GPIOD_BASE_ADDR)
#define GPIOE_REG ((volatile GPIO_Register_Offset_t*)GPIOE_BASE_ADDR)
#define GPIOH_REG ((volatile GPIO_Register_Offset_t*)GPIOH_BASE_ADDR)

/*
 * EXTI Reg Pointers
 */
#define EXTI_REG ((volatile EXTI_Register_Offset_t*)EXTI_BASE_ADDR)

/*
 * UART Reg Pointers
 */
#define UART1_REG ((volatile UART_Register_Offset_t*)UART1_BASE_ADDR)
#define UART2_REG ((volatile UART_Register_Offset_t*)UART2_BASE_ADDR)
#define UART6_REG ((volatile UART_Register_Offset_t*)UART6_BASE_ADDR)

/*
 * SPI Reg Pointers
 */
#define SPI1_REG ((volatile SPI_Register_Offset_t*)SPI1_BASE_ADDR)
#define SPI2_REG ((volatile SPI_Register_Offset_t*)SPI2_BASE_ADDR)
#define SPI3_REG ((volatile SPI_Register_Offset_t*)SPI3_BASE_ADDR)
#define SPI4_REG ((volatile SPI_Register_Offset_t*)SPI4_BASE_ADDR)
#define SPI5_REG ((volatile SPI_Register_Offset_t*)SPI5_BASE_ADDR)

/*
 * TIMER1 Reg Pointers
 */
#define TIM1_REG ((volatile TIM_Register_Offset_t*)TIM1_BASE_ADDR)
#define TIM2_REG ((volatile TIM_Register_Offset_t*)TIM2_BASE_ADDR)
#define TIM3_REG ((volatile TIM_Register_Offset_t*)TIM3_BASE_ADDR)
#define TIM4_REG ((volatile TIM_Register_Offset_t*)TIM4_BASE_ADDR)
#define TIM5_REG ((volatile TIM_Register_Offset_t*)TIM5_BASE_ADDR)

#define TIM9_REG ((volatile TIM_Register_Offset_t*)TIM9_BASE_ADDR)
#define TIM10_REG ((volatile TIM_Register_Offset_t*)TIM10_BASE_ADDR)
#define TIM11_REG ((volatile TIM_Register_Offset_t*)TIM11_BASE_ADDR)

/*
 * Flash Reg Pointers
 */
#define FLASH_REG ((Flash_IntF_Register_Offset_t*) FLASH_INTF_REG_ADDR)

/*
 * NVIC Reg Pointers
 */
#define NVIC_REG ((volatile NVIC_t*) NVIC_BASE_ADDR)

/*
 * RCC Reg Pointers
 */
#define RCC_REG ((volatile RCC_Register_Offset_t*) RCC_BASE_ADDR);


////////////END OF REGISTER POINTERS////////////



////////////END OF ENUM////////////



#endif /* INC_RES_ADDR_H_ */
