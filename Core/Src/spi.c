/*
 * flash.c
 *
 *  Created on: Jun 11, 2025
 *      Author: dobao
 */

#include "spi.h"

void SPI_Init(GPIO_Pin_t mosiPin, GPIO_Pin_t misoPin, GPIO_Pin_t sckPin, GPIO_PortName_t portName, SPI_Name_t SPIx){
	//Flexible enable GPIO Clock
	switch(portName){
		case my_GPIOA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
		case my_GPIOB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
		case my_GPIOC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
		case my_GPIOD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
		case my_GPIOE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
		case my_GPIOH: __HAL_RCC_GPIOH_CLK_ENABLE(); break;
		default: return;
	}

	//Flexible enable SPI clock
	switch(SPIx){
		case my_SPI1: __HAL_RCC_SPI1_CLK_ENABLE(); break;
		case my_SPI2: __HAL_RCC_SPI2_CLK_ENABLE(); break;
		case my_SPI3: __HAL_RCC_SPI3_CLK_ENABLE(); break;
		case my_SPI4: __HAL_RCC_SPI4_CLK_ENABLE(); break;
		case my_SPI5: __HAL_RCC_SPI5_CLK_ENABLE(); break;
		default: return;
	}

	/*
	 * Config these pins to Alternate Function Mode for SPI interface
	 */
	GPIO_WritePin(sckPin, portName, MODER, mode_02);
	GPIO_WritePin(mosiPin, portName, MODER, mode_02);
	GPIO_WritePin(misoPin, portName, MODER, mode_02);

	/*
	 * Condition to check which AF reg (AFRL or AFRH) is chosen corresponding to pin number
	 */
	GPIO_Mode_t afrRegSck = (sckPin <= 7) ? AFRL : AFRH;
	GPIO_Mode_t afrRegMosi = (mosiPin <= 7) ? AFRL : AFRH;
	GPIO_Mode_t afrRegMiso = (misoPin <= 7) ? AFRL : AFRH;

	/*
	 * SPIx to Alternate Function Mapping (STM32F411VET)
	 * Conditions to check which AFx order is chosen corresponding to sckPin, portName, and SPIx
	 * SPI1/2/4 -> AF5
	 * SPI3 -> AF6 (PB12 special case -> AF7)
	 * SPI5 -> AF6
	 */
	if(sckPin == 12 && portName == my_GPIOB){
		GPIO_WritePin(sckPin, portName, afrRegSck, AF7); //set PB12 to AF07 (SPI3_SCK);
	}
	else if(SPIx == my_SPI1 || SPIx == my_SPI2 || SPIx == my_SPI4){
		GPIO_WritePin(sckPin, portName, afrRegSck, AF5); //Set to SPI1/2/4_SCK
	}
	else{
		GPIO_WritePin(sckPin, portName, afrRegSck, AF6); //Set to SPI3/5_SCK
	}

	/*
	 * SPIx to Alternate Function Mapping (STM32F411VET)
	 * Conditions to check which AFx order is chosen corresponding to mosiPin, portName, and SPIx
	 * SPI3/5 -> AF6 (PD6 special case -> AF5)
	 * SPI1/2/4 -> AF5
	 */
	if(SPIx == my_SPI5 || SPIx == my_SPI3){
		if(mosiPin == my_GPIO_PIN_6 && portName == my_GPIOD){
			GPIO_WritePin(mosiPin, portName, afrRegMosi, AF5);
		}
		else{
			GPIO_WritePin(mosiPin, portName, afrRegMosi, AF6);
		}
	}
	else{
		GPIO_WritePin(mosiPin, portName, afrRegMosi, AF5);
	}

	/*
	 * Conditions to check which AFx order is chosen corresponding to misoPin, portName, and SPIx
	 * SPI1/2/4 -> AF5 (PA11 special case -? AF6)
	 * SPI3/5 -> AF6
	 */
	if(SPIx == my_SPI1 || SPIx == my_SPI2 || SPIx == my_SPI4){
		if(misoPin == my_GPIO_PIN_11 && portName == my_GPIOA){
			GPIO_WritePin(misoPin, portName, afrRegMiso, AF6);
		}
		else{
			GPIO_WritePin(misoPin, portName, afrRegMiso, AF5);
		}
	}
	else if(SPIx == my_SPI3 || SPIx == my_SPI5){
		GPIO_WritePin(misoPin, portName, afrRegMiso, AF6);
	}
}
