/*
 * adc.c
 *
 *  Created on: Jul 6, 2025
 *      Author: dobao
 */

#include "adc.h"

#define ADC_BASE_ADDR 0x40012000U
#define longPointerType volatile uint32_t*

static volatile uint32_t* ADC_SR = (volatile uint32_t*)ADC_BASE_ADDR + 0x00;

static volatile uint32_t* ADC_CR1 = (volatile uint32_t*)ADC_BASE_ADDR + 0x04;
static volatile uint32_t* ADC_CR2 = (volatile uint32_t*)ADC_BASE_ADDR + 0x08;

static volatile uint32_t* ADC_SMPR1 = (volatile uint32_t*)ADC_BASE_ADDR + 0x0C;
static volatile uint32_t* ADC_SMPR2 = (volatile uint32_t*)ADC_BASE_ADDR + 0x10;

static volatile uint32_t* ADC_JOFR1 = (volatile uint32_t*)ADC_BASE_ADDR + 0x14;
static volatile uint32_t* ADC_JOFR2 = (volatile uint32_t*)ADC_BASE_ADDR + 0x18;
static volatile uint32_t* ADC_JOFR3 = (volatile uint32_t*)ADC_BASE_ADDR + 0x1C;
static volatile uint32_t* ADC_JOFR4 = (volatile uint32_t*)ADC_BASE_ADDR + 0x20;

static volatile uint32_t* ADC_HTR = (volatile uint32_t*)ADC_BASE_ADDR + 0x24;
static volatile uint32_t* ADC_LTR = (volatile uint32_t*)ADC_BASE_ADDR + 0x28;

static volatile uint32_t* ADC_SQR1 = (volatile uint32_t*)ADC_BASE_ADDR + 0x2C;
static volatile uint32_t* ADC_SQR2 = (volatile uint32_t*)ADC_BASE_ADDR + 0x30;
static volatile uint32_t* ADC_SQR3 = (volatile uint32_t*)ADC_BASE_ADDR + 0x34;

static volatile uint32_t* ADC_JSQR = (volatile uint32_t*)ADC_BASE_ADDR + 0x38;

static volatile uint32_t* ADC_JDR1 = (volatile uint32_t*)ADC_BASE_ADDR + 0x3C;
static volatile uint32_t* ADC_JDR2 = (volatile uint32_t*)ADC_BASE_ADDR + 0x40;
static volatile uint32_t* ADC_JDR3 = (volatile uint32_t*)ADC_BASE_ADDR + 0x44;
static volatile uint32_t* ADC_JDR4 = (volatile uint32_t*)ADC_BASE_ADDR + 0x48;

static volatile uint32_t* ADC_DR = (volatile uint32_t*)ADC_BASE_ADDR + 0x4C;
static volatile uint32_t* ADC_CCR = (longPointerType) ADC_BASE_ADDR + 0x04 + 0x300;


void ADC_init(){
	my_RCC_ADC1_CLK_ENABLE();
	/*
	 * ADC Clock support 36MHz max (datasheet)
	 * APB2 clock is now 50MHz which is > ADC Clock
	 * Therefore, use prescaler of 2 to 50/2 = 25MHz for ADC Clock
	 */
	*ADC_CCR &= ~(0b11 << 16);
	*ADC_CCR |= (0b00 << 16); //Set prescaler to 2 so that PCLK2 / 2 <= 36MHz

	*ADC_JSQR &= ~(0b11 << 20); //clear bits
	*ADC_JSQR |= (0b00 << 20); //write 0b00 to JL[1:0] to choose 1 conversion

	*ADC_JSQR &= ~(0b1111 << 0);
	*ADC_JSQR |= (16 << 0);

	*ADC_CCR |= (1 << 23); //Enabling temperature sensor



}
