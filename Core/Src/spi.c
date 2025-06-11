///*
// * spi.c
// *
// *  Created on: Jun 1, 2025
// *      Author: dobao
// */
//#include "registerAddress.h"
//#include "gpio_write_read.h"
//#include "stm32f4xx_hal.h"
//
//void SPI_Init(char MOSIpin, char MISOpin, char SCLKpin, char CSpin, GPIO_PortName_t GPIOx){
//	GPIO_WritePin(SCLKpin, GPIOx, MODER, 0b10); //set pin to Alternate Function
//	GPIO_WritePin(SCLKpin, GPIOx, AFRL, 0b0101); //Set pin to SP1_SCLK
//
//	GPIO_WritePin(MOSIpin, GPIOx, MODER, 0b10); //Set PA7 to Alternate Function
//	GPIO_WritePin(MOSIpin, GPIOx, AFRL, 0b0101); //Set PA7 to SPI1_MOSI
//
//	GPIO_WritePin(MISOpin, GPIOx, MODER, 0b10); //Set pin to Alternate Function
//	GPIO_WritePin(MISOpin, GPIOx, AFRL, 0b0101); //Set pin to SP1_MISO
//
//	GPIO_WritePin(CSpin, my_GPIOE, MODER, 0b01); //Set pin to output mode
//
//	__HAL_RCC_SPI1_CLK_ENABLE();
//	SPI_Register_Offset_t* SPI1x = SPI1_REG;
//
//	SPI1x -> SPI_CR1 &= ~(0b111 << 3); //Clear bits
//
//	SPI1x -> SPI_CR1 |= (1 << 2) | (1 << 9) | (1 << 6) | (0b111 << 3); //Config STM32 as master, Software slave management enable, SPI enable, f_PCLK/256 baud rate control)
//}
//
//char SPI_read(char reg_addr_slave){
//	GPIO_WritePin(3, my_GPIOE, ODR, 0); //Set active low for PE3
//
//	//Send slave's address to DR
//	SPI_Register_Offset_t* SPI1x = SPI1_REG;
//	SPI1x -> SPI_DR |= reg_addr_slave;
//
//	while((SPI1x -> SPI_SR))
//
//	//read DR to clear the garbage value
//
//}
