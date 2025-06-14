/*
 * spi.h
 *
 *  Created on: Jun 11, 2025
 *      Author: dobao
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "gpio_write_read.h"
#include "registerAddress.h"


/*
 * Collections of SPIx name
 */
typedef enum{
	my_SPI1,
	my_SPI2,
	my_SPI3,
	my_SPI4,
	my_SPI5
}SPI_Name_t;

/*
 * Collections of SPI Offset Register Name
 */
typedef enum{SPI_CR1, SPI_CR2, SPI_SR, SPI_DR, SPI_CRC,
			SPI_RXCRCR, SPI_TXCRCR, SPI_I2SCFGR, SPI_I2SPR}SPI_Mode_t;

/*
 * Names of several features for basic SPI configuring
 */
typedef enum{
	SOFTWARE_SLAVE_DISABLE,
	SOFTWARE_SLAVE_ENABLE//Remember to set SSI to 1 when user choose this to prevent activating MODF
}SPI_SSM_t; //Software Slave Management

typedef enum{
	SPI_DISABLE,
	SPI_ENABLE
}SPI_Enable_t;

typedef enum{
	FPCLK_DIV2 = 0b000, //freq/2 = 16MHz/2
	FPCLK_DIV4 = 0b001,
	FPCLK_DIV8 = 0b010,
	FPCLK_DIV16 = 0b011, //freq/16 = 16MHz/16
	FPCLK_DIV32 = 0b100,
	FPCLK_DIV64 = 0b101,
	FPCLK_DIV128 = 0b110,
	FPCLK_DIV2256 = 0b111
}SPI_BaudRate_t;

typedef enum{
	STM32_SLAVE,
	STM32_MASTER
}SPI_MSTR_t; //Master selection



/*
 * Function Declarations
 */
void WriteSPI(uint8_t bitPosition, SPI_Name_t userSPIx, SPI_Mode_t mode, uint32_t value);
void SPI_sckPin_Init(GPIO_Pin_t sckPin, GPIO_PortName_t sckPort, SPI_Name_t SPIx);
void SPI_mosiPin_Init(GPIO_Pin_t mosiPin, GPIO_PortName_t mosiPort, SPI_Name_t SPIx);
void SPI_misoPin_Init(GPIO_Pin_t misoPin, GPIO_PortName_t misoPort, SPI_Name_t SPIx);

void SPI_GPIO_Init(SPI_Name_t SPIx,
			  GPIO_Pin_t sckPin, GPIO_PortName_t sckPort,
			  GPIO_Pin_t mosiPin, GPIO_PortName_t mosiPort,
			  GPIO_Pin_t misoPin, GPIO_PortName_t misoPort);

void SPI_basicConfigInit(SPI_Name_t SPIx, SPI_MSTR_t masterSlaveSel,
						SPI_Enable_t enableMode, SPI_BaudRate_t baudRateSel,
						SPI_SSM_t softSlaveEn);

#endif /* INC_SPI_H_ */
