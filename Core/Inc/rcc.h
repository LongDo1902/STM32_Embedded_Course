/*
 * RCC.h
 *
 *  Created on: Jun 30, 2025
 *      Author: dobao
 */

#ifndef INC_RCC_H_
#define INC_RCC_H_

#include <stdio.h>
#include <stdbool.h>
#include "timer.h"
#include "stm32f4xx.h"
#include "registerAddress.h"
#include "flash.h"


#define HSERDY_TIMEOUT	0x2000U
#define PLLRDY_TIMEOUT	0x4000U
#define	SWS_TIMEOUT		0x4000U

#define GET_RCC_REG(mode) (&(RCC_REG -> mode))

/*
 * List of RCC Registers
 */
typedef enum{
	RCC_CR,			//RCC Clock Control Reg (Ctrl main oscillators and PLL, managing HSI, HSE, PLL)
	RCC_PLL_CFGR,	//RCC PLL Config Reg (Config PLL source and multiplication/division factors)
	RCC_CFGR,		//RCC Clk Ctrl Reg (Used to select the sys clk src, config AHB, APB buses and other clk div settings)
	RCC_CIR,		//RCC Clk Interrupt Reg (Manages interrupt flags related to RCC events, detects issues with clk srcs or PLLs)

	RCC_AHB1_RSTR,
	RCC_AHB2_RSTR,

	RCC_APB1_RSTR,
	RCC_APB2_RSTR,

	RCC_AHB1_ENR,
	RCC_AHB2_ENR,

	RCC_APB1_ENR,
	RCC_APB2_ENR,

	RCC_AHB1_LP_ENR,
	RCC_AHB2_LP_ENR,

	RCC_APB1_LP_ENR,
	RCC_APB2_LP_ENR,

	RCC_BDCR,
	RCC_CSR,

	RCC_SSCGR,
	RCC_PLL_I2S_CFGR,

	RCC_DCK_CFGR,

	RCC_REG_COUNT	//A parameter to store the total number of registers / reflects the array size
}RCC_Name_t;



/*
 * LIST OF FUNCTION DECLARATIONS
 */
void RCC_init(void);
void writeRCC(uint8_t bitPosition, RCC_Name_t mode, uint32_t value);
uint32_t readRCC(uint8_t bitPosition, RCC_Name_t mode);
void writeBits(volatile uint32_t* reg, uint8_t bitPosition, uint8_t bitWidth, uint32_t value);



/*
 * ENABLE AND DISABLE TIMS' CLOCK
 */
void my_RCC_TIM1_CLK_ENABLE();
void my_RCC_TIM1_CLK_DISABLE();

void my_RCC_TIM2_CLK_ENABLE();
void my_RCC_TIM2_CLK_DISABLE();

void my_RCC_TIM3_CLK_ENABLE();
void my_RCC_TIM3_CLK_DISABLE();

void my_RCC_TIM4_CLK_ENABLE();
void my_RCC_TIM4_CLK_DISABLE();

void my_RCC_TIM5_CLK_ENABLE();
void my_RCC_TIM5_CLK_DISABLE();

void my_RCC_TIM9_CLK_ENABLE();
void my_RCC_TIM9_CLK_DISABLE();

void my_RCC_TIM10_CLK_ENABLE();
void my_RCC_TIM10_CLK_DISABLE();

void my_RCC_TIM11_CLK_ENABLE();
void my_RCC_TIM11_CLK_DISABLE();



/*
 * ENABLE AND DISABLE GPIOS' CLOCK
 */
void my_RCC_GPIOA_CLK_ENABLE();
void my_RCC_GPIOA_CLK_DISABLE();

void my_RCC_GPIOB_CLK_ENABLE();
void my_RCC_GPIOB_CLK_DISABLE();

void my_RCC_GPIOC_CLK_ENABLE();
void my_RCC_GPIOC_CLK_DISABLE();

void my_RCC_GPIOD_CLK_ENABLE();
void my_RCC_GPIOD_CLK_DISABLE();

void my_RCC_GPIOE_CLK_ENABLE();
void my_RCC_GPIOE_CLK_DISABLE();

void my_RCC_GPIOH_CLK_ENABLE();
void my_RCC_GPIOH_CLK_DISABLE();



/*
 * ENABLE AND DISABLE SPIS' CLOCK
 */
void my_RCC_SPI1_CLK_ENABLE();
void my_RCC_SPI1_CLK_DISABLE();

void my_RCC_SPI2_CLK_ENABLE();
void my_RCC_SPI2_CLK_DISABLE();

void my_RCC_SPI3_CLK_ENABLE();
void my_RCC_SPI3_CLK_DISABLE();

void my_RCC_SPI4_CLK_ENABLE();
void my_RCC_SPI4_CLK_DISABLE();

void my_RCC_SPI5_CLK_ENABLE();
void my_RCC_SPI5_CLK_DISABLE();



/*
 * ENABLE AND DISABLE USARTS' CLOCK
 */
void my_RCC_USART1_CLK_ENABLE();
void my_RCC_USART1_CLK_DISABLE();

void my_RCC_USART2_CLK_ENABLE();
void my_RCC_USART2_CLK_DISABLE();

void my_RCC_USART6_CLK_ENABLE();
void my_RCC_USART6_CLK_DISABLE();



/*
 * ENABLE AND DISBALE I2CS' CLOCK
 */
void my_RCC_I2C1_CLK_ENABLE();
void my_RCC_I2C1_CLK_DISABLE();

void my_RCC_I2C2_CLK_ENABLE();
void my_RCC_I2C2_CLK_DISABLE();

void my_RCC_I2C3_CLK_ENABLE();
void my_RCC_I2C3_CLK_DISABLE();


#endif /* INC_RCC_H_ */













