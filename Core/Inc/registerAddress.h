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


//Base Addresses for GPIO ports A-H
//UL is unsigned Long
#define GPIOA_BASE_ADDR 0x40020000UL
#define GPIOB_BASE_ADDR 0x40020400UL
#define GPIOC_BASE_ADDR 0x40020800UL
#define GPIOD_BASE_ADDR 0x40020C00UL
#define GPIOE_BASE_ADDR 0x40021000UL
#define GPIOH_BASE_ADDR 0x40021C00UL


//GPIO Registers Offset Value
typedef struct{
	volatile uint32_t MODER; 	//0x00 (Mode Register)
	volatile uint32_t OTYPER; 	//0x04 (Output Type Register)
	volatile uint32_t OSPEEDR; 	//0x08 (Output Speed Register)
	volatile uint32_t PUPDR;	//0x0C (Pull-up/pull-down Register)
	volatile uint32_t IDR;		//0x10 (Input Data Register)
	volatile uint32_t ODR;		//0x14 (Output Data Register)
	volatile uint32_t BSRR;		//0x18 (Bit Set/Reset Register)
}GPIO_Register_Offset_t;


//Make a pointer called GPIOx_REG points to this memory layout at addr GPIOx_BASE_ADDR
#define GPIOA_REG ((GPIO_Register_Offset_t*)GPIOA_BASE_ADDR)
#define GPIOB_REG ((GPIO_Register_Offset_t*)GPIOB_BASE_ADDR)
#define GPIOC_REG ((GPIO_Register_Offset_t*)GPIOC_BASE_ADDR)
#define GPIOD_REG ((GPIO_Register_Offset_t*)GPIOD_BASE_ADDR)
#define GPIOE_REG ((GPIO_Register_Offset_t*)GPIOE_BASE_ADDR)
#define GPIOH_REG ((GPIO_Register_Offset_t*)GPIOH_BASE_ADDR)


//EXTI Registers
#define EXTI_BASE_ADDR	0x40013C00UL


//External Interrupt Registers Offset Value
typedef struct{
	volatile uint32_t IMR; 		//0x00 (Interrupt Mask Register)
	volatile uint32_t EMR; 		//0x04 (Event Mask Register)
	volatile uint32_t RTSR;		//0x08 (Rising Trigger Selection Register)
	volatile uint32_t FTSR;		//0x0C (Falling Trigger Selection Register)
	volatile uint32_t SWIER; 	//0x10 (Software Interrupt Event Register)
	volatile uint32_t PR;		//0x14 (Pending Register)
}EXTI_Register_Offset_t;


//External Interrupt stores bit positions in NVIC
typedef enum{
	EXTI0 = 6, //External Interrupt 0
	EXTI1, //...1
	EXTI2, //...2
	EXTI3, //...3
	EXTI4  //...4
}EXTI_Num_t;


//External Interrupt stores their address
typedef enum{
	EXTI0_ADDR = 0x58,

}EXTI_Addr_t;


//NVIC - Interrupt Set-Enable Register (Cortex-M4 ref manual)
typedef enum{
	NVIC_ISER0 = 0xE000E100,
	NVIC_ISER1 = 0xE000E104,
	NVIC_ISER2 = 0xE000E108,
	NVIC_ISER3 = 0xE000E10C,
	NVIC_ISER4 = 0xE000E110,
	NVIC_ISER5 = 0xE000E114,
	NVIC_ISER6 = 0xE000E118,
	NVIC_ISER7 = 0xE000E11C
}NVIC_ISER_t;


//Create a new definition and it is a pointer to a struct name EXTI_Register_Offset_t locate at base memory address
#define EXTI_REG ((EXTI_Register_Offset_t*)EXTI_BASE_ADDR)


//VTOR (Vector Table Offset Register) (Cortex-M4 ref manual)
//#define VTO_REG ((volatile uint32_t*)(0xE000ED08))

//////////////////////////////END OF REGISTER//////////////////////////////
/*
 * Function Declarations
 */
void EXTI_Init(char pin, NVIC_ISER_t NVIC_ISERx, EXTI_Num_t EXTIx);
void EXTI_Offset_Init(); //Init EXTI offset



#endif /* INC_RES_ADDR_H_ */
