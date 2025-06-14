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


void SPI_Init(GPIO_Pin_t mosiPin, GPIO_Pin_t misoPin,
			GPIO_Pin_t sckPin, GPIO_PortName_t portName, SPI_Name_t SPIx);

#endif /* INC_SPI_H_ */
