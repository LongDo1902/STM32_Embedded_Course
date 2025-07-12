/*
 * adc.c
 *
 *  Created on: Jul 6, 2025
 *      Author: dobao
 */

#include "adc.h"

#define ADC_BASE_ADDR 0x40012000U
#define longPointerType volatile uint32_t*

static volatile uint32_t* ADC_SR = (volatile uint32_t*)(ADC_BASE_ADDR + 0x00);

static volatile uint32_t* ADC_CR1 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x04);
static volatile uint32_t* ADC_CR2 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x08);

static volatile uint32_t* ADC_SMPR1 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x0C);
static volatile uint32_t* ADC_SMPR2 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x10);

static volatile uint32_t* ADC_JOFR1 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x14);
static volatile uint32_t* ADC_JOFR2 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x18);
static volatile uint32_t* ADC_JOFR3 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x1C);
static volatile uint32_t* ADC_JOFR4 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x20);

static volatile uint32_t* ADC_HTR = (volatile uint32_t*)(ADC_BASE_ADDR + 0x24);
static volatile uint32_t* ADC_LTR = (volatile uint32_t*)(ADC_BASE_ADDR + 0x28);

static volatile uint32_t* ADC_SQR1 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x2C);
static volatile uint32_t* ADC_SQR2 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x30);
static volatile uint32_t* ADC_SQR3 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x34);

static volatile uint32_t* ADC_JSQR = (volatile uint32_t*)(ADC_BASE_ADDR + 0x38);

static volatile uint32_t* ADC_JDR1 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x3C);
static volatile uint32_t* ADC_JDR2 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x40);
static volatile uint32_t* ADC_JDR3 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x44);
static volatile uint32_t* ADC_JDR4 = (volatile uint32_t*)(ADC_BASE_ADDR + 0x48);

static volatile uint32_t* ADC_DR = (volatile uint32_t*)(ADC_BASE_ADDR + 0x4C);
static volatile uint32_t* ADC_CCR = (volatile uint32_t*)(ADC_BASE_ADDR + 0x04 + 0x300);


void ADC_tempSensorInit(){
	my_RCC_ADC1_CLK_ENABLE();
	/*
	 * ADC Clock support 36MHz max (datasheet)
	 * APB2 clock is now 50MHz which is > ADC Clock
	 * Therefore, use prescaler of 2 to 50/2 = 25MHz for ADC Clock
	 */
	*ADC_CCR &= ~(0b11 << 16);
	*ADC_CCR |= (0b00 << 16); //Set prescaler to 2 so that PCLK2 / 2 <= 36MHz

	/*
	 * t_cycle = 1/25MHz = 40ns per ADC clock cycle
	 * datasheet says min sampling time when reading the temperature sensor is 10us
	 *
	 * -> 10us / 40ns = 250 cycles
	 * Therefore, choose 0b111: 480 cycles in ADC_SMPR1 for channel 16
	 */
	*ADC_SMPR1 &= ~(0b111 << 18); //SMP16[2:0] starts at bit 18th
	*ADC_SMPR1 |= (0b111 << 18); //480 cycles

	*ADC_JSQR &= ~(0b11 << 20); //clear bits
	*ADC_JSQR |= (0b00 << 20); //write 0b00 to JL[1:0] to choose 1 conversion

	*ADC_JSQR &= ~(0b1111 << 0);
	*ADC_JSQR |= (16 << 16); //Select JSQ4[4:1] start at bit 16

	*ADC_CCR |= (1 << 23); //Enabling temperature sensor

	*ADC_CR2 |= (1 << 0); //Enable ADC
}


/*
 *
 */
float tempSensorRead(){
	*ADC_CR2 |= (1 << 22); //JSWSTART Starts conversion of injected channels
	while(((*ADC_SR >> 2) & 1u) == 0); //Wait until injected channel conversion is completed
	*ADC_SR &= ~(1u << 2); //Clear the bit since the ref man says it is cleared by software
	uint16_t adcTempRaw = *ADC_JDR1;
	/*
	 * Vref 3.0		4095 (12bits)
	 * Vin ?		adcTempRaw
	 */
	float vin = (adcTempRaw * 3.0) / 4095;
	float temperature = ((vin - 0.76) / 0.0025) + 25;
	return temperature;
}







