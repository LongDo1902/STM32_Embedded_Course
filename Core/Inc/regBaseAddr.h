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

//Base address from Port A to H
#define GPIOA_BASE_ADDR 0x40020000
#define GPIOB_BASE_ADDR 0x40020400
#define GPIOC_BASE_ADDR 0x40020800
#define GPIOD_BASE_ADDR 0x40020C00
#define GPIOE_BASE_ADDR 0x40021000
#define GPIOH_BASE_ADDR 0x40021C00

#define MODER_OFFSET	0x00
#define OTYPER_OFFSET 	0x04
#define OSPEEDR_OFFSET 	0x08
#define PUPDR_OFFSET 	0x0C
#define IDR_OFFSET 		0x10
#define ODR_OFFSET 		0x14
#define BSRR_OFFSET 	0x18


//GPIOs Moder (Mode Register)
#define GPIOA_MODER ((volatile uint32_t*)(GPIOA_BASE_ADDR + MODER_OFFSET))
#define GPIOB_MODER ((volatile uint32_t*)(GPIOB_BASE_ADDR + MODER_OFFSET))
#define GPIOC_MODER ((volatile uint32_t*)(GPIOC_BASE_ADDR + MODER_OFFSET))
#define GPIOD_MODER ((volatile uint32_t*)(GPIOD_BASE_ADDR + MODER_OFFSET))
#define GPIOE_MODER ((volatile uint32_t*)(GPIOE_BASE_ADDR + MODER_OFFSET))
#define GPIOH_MODER ((volatile uint32_t*)(GPIOH_BASE_ADDR + MODER_OFFSET))


//GPIOs BSRR (Bit Set and Reset Register)
#define GPIOA_BSRR ((volatile uint32_t*)(GPIOA_BASE_ADDR + BSRR_OFFSET))
#define GPIOB_BSRR ((volatile uint32_t*)(GPIOB_BASE_ADDR + BSRR_OFFSET))
#define GPIOC_BSRR ((volatile uint32_t*)(GPIOC_BASE_ADDR + BSRR_OFFSET))
#define GPIOD_BSRR ((volatile uint32_t*)(GPIOD_BASE_ADDR + BSRR_OFFSET))
#define GPIOE_BSRR ((volatile uint32_t*)(GPIOE_BASE_ADDR + BSRR_OFFSET))
#define GPIOH_BSRR ((volatile uint32_t*)(GPIOH_BASE_ADDR + BSRR_OFFSET))

//GPIOs ODR (Output Data Register)
#define GPIOA_ODR ((volatile uint32_t*)(GPIOA_BASE_ADDR + ODR_OFFSET))
#define GPIOB_ODR ((volatile uint32_t*)(GPIOB_BASE_ADDR + ODR_OFFSET))
#define GPIOC_ODR ((volatile uint32_t*)(GPIOC_BASE_ADDR + ODR_OFFSET))
#define GPIOD_ODR ((volatile uint32_t*)(GPIOD_BASE_ADDR + ODR_OFFSET))
#define GPIOE_ODR ((volatile uint32_t*)(GPIOE_BASE_ADDR + ODR_OFFSET))
#define GPIOH_ODR ((volatile uint32_t*)(GPIOE_BASE_ADDR + ODR_OFFSET))

//GPIOs PUPDR (pull-up or pull-down register)
#define GPIOA_PUPDR ((volatile uint32_t*)(GPIOA_BASE_ADDR + PUPDR_OFFSET))
#define GPIOB_PUPDR ((volatile uint32_t*)(GPIOB_BASE_ADDR + PUPDR_OFFSET))
#define GPIOC_PUPDR ((volatile uint32_t*)(GPIOC_BASE_ADDR + PUPDR_OFFSET))
#define GPIOD_PUPDR ((volatile uint32_t*)(GPIOD_BASE_ADDR + PUPDR_OFFSET))
#define GPIOE_PUPDR ((volatile uint32_t*)(GPIOE_BASE_ADDR + PUPDR_OFFSET))
#define GPIOH_PUPDR ((volatile uint32_t*)(GPIOH_BASE_ADDR + PUPDR_OFFSET))

//GPIOs IDR (Input Data Register)
#define GPIOA_IDR ((volatile uint32_t*)(GPIOA_BASE_ADDR + IDR_OFFSET))
#define GPIOB_IDR ((volatile uint32_t*)(GPIOB_BASE_ADDR + IDR_OFFSET))
#define GPIOC_IDR ((volatile uint32_t*)(GPIOC_BASE_ADDR + IDR_OFFSET))
#define GPIOD_IDR ((volatile uint32_t*)(GPIOD_BASE_ADDR + IDR_OFFSET))
#define GPIOE_IDR ((volatile uint32_t*)(GPIOE_BASE_ADDR + IDR_OFFSET))
#define GPIOH_IDR ((volatile uint32_t*)(GPIOH_BASE_ADDR + IDR_OFFSET))




#endif /* INC_RES_ADDR_H_ */
