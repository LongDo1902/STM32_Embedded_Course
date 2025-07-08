///*
// * adc.c
// *
// *  Created on: Jul 6, 2025
// *      Author: dobao
// */
//#include <stdint.h>
//#include <stdio.h>
//
//#define ADC_BASE_ADDR 0x40012000
//
//uint32_t* ADC_SR = (uint32_t*) ADC_BASE_ADDR + 0x00;
//uint32_t* ADC_CR1 = (uint32_t*) ADC_BASE_ADDR + 0x04;
//uint32_t* ADC_CR2 = (uint32_t*) ADC_BASE_ADDR + 0x08;
//uint32_t* ADC_SMPR1 = (uint32_t*) ADC_BASE_ADDR + 0x0C;
//uint32_t* ADC_JSQR = (uint32_t*) ADC_BASE_ADDR + 0x38;
//uint32_t* ADC_JDR1 = (uint32_t*) ADC_BASE_ADDR + 0x3C;
//uint32_t* ADC_CCR = (uint32_t*) ADC_BASE_ADDR + 0x300 + 0x04;
//
//void initADC(){
//	ADC_JSQR &= ~(0b11 << 20); //clear these bits before writing
//	ADC_JSQR |= (0b00 << 20);
//	ADC_JSQR &= ~()
//
//}
