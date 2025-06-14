/*
 * flash.c
 *
 *  Created on: Jun 11, 2025
 *      Author: dobao
 */

#include "spi.h"

/*
 *  @brief	Initializes the selected SPI peripheral and its SCK, MOSI, MISO pins.
 */
void SPI_GPIO_Init(SPI_Name_t SPIx,
			  GPIO_Pin_t sckPin, GPIO_PortName_t sckPort,
			  GPIO_Pin_t mosiPin, GPIO_PortName_t mosiPort,
			  GPIO_Pin_t misoPin, GPIO_PortName_t misoPort){

	Enable_GPIO_Clock(sckPort);
	Enable_GPIO_Clock(mosiPort);
	Enable_GPIO_Clock(misoPort);

	SPI_sckPin_Init(sckPin, sckPort, SPIx);
	SPI_mosiPin_Init(mosiPin, mosiPort, SPIx);
	SPI_misoPin_Init(misoPin, misoPort, SPIx);
}


/*
 * @brief	Initialize the basic configuration of an SPI peripheral
 *
 * This includes:
 * 		Clock enabled for the selected SPI instance
 * 		Master/Slave mode selection
 * 		Baud rate configuration
 * 		Software slave management (SSM) and SSI
 * 		SPI peripheral enable
 *
 * @param	SPIx				Selected SPI peripheral (my_SPI1 to my_SPI5)
 * @param	masterSlaveSel		Set device as master or slave (SPI_MSTR_t)
 * @param	enableMode			Enable or disable SPI (SPI_Enable_t)
 * @param	baudRateSel			Baud rate prescaler (SPI_BaudRate_t)
 * @param	softSlaveEn 		Software slave management enable/disable (SPI_SSM_t)
 */
void SPI_basicConfigInit(SPI_Name_t SPIx, SPI_MSTR_t masterSlaveSel,
						SPI_Enable_t enableMode, SPI_BaudRate_t baudRateSel,
						SPI_SSM_t softSlaveEn){

	//Flexible enable SPI clock
	switch(SPIx){
		case my_SPI1: __HAL_RCC_SPI1_CLK_ENABLE(); break; //16MHz
		case my_SPI2: __HAL_RCC_SPI2_CLK_ENABLE(); break;
		case my_SPI3: __HAL_RCC_SPI3_CLK_ENABLE(); break;
		case my_SPI4: __HAL_RCC_SPI4_CLK_ENABLE(); break;
		case my_SPI5: __HAL_RCC_SPI5_CLK_ENABLE(); break;
		default: return;
	}

	WriteSPI(2, SPIx, SPI_CR1, masterSlaveSel); //Set STM32F411VET as master or slave
	WriteSPI(3, SPIx, SPI_CR1, baudRateSel); //Set how fast sckPin can generate (Hz)
	WriteSPI(9, SPIx, SPI_CR1, softSlaveEn);//Needed if NSS is not physically connected

	/*
	 * bit 8 = 1 -> Simulate NSS being high so that MODF in SR is disabled
	 * bit 8 = 0 -> physical NSS is selected/connected
	 */
	WriteSPI(8, SPIx, SPI_CR1, softSlaveEn == SOFTWARE_SLAVE_ENABLE ? 1 : 0);

	//Enable SPI must be put after all other features are activated
	WriteSPI(6, SPIx, SPI_CR1, enableMode); //Disable or Enable SPI
}



/*
 * @brief	Initializes the SCK pin for a given SPI peripheral.
 * @param	sckPin		SPI clock pin
 * @param	sckPort		Port of the SCK pin
 * @param	SPIx		Target SPI (e.g., my_SPI1)
 */
void SPI_sckPin_Init(GPIO_Pin_t sckPin, GPIO_PortName_t sckPort, SPI_Name_t SPIx){
	GPIO_WritePin(sckPin, sckPort, MODER, mode_02); //Config this pin in MODER to Alternate Function mode for SPI interface
	GPIO_Mode_t afrRegSck = (sckPin <= 7) ? AFRL : AFRH; //Check which AF reg (AFRL or AFRH) is chosen corresponding to pin number

	/*
	 * SPIx to Alternate Function Mapping (STM32F411VET)
	 * Conditions to check which AFx order is chosen corresponding to sckPin, sckPort, and SPIx
	 * SPI1/2/4 -> AF5
	 * SPI3 -> AF6 (PB12 special case -> AF7)
	 * SPI5 -> AF6
	 */
	if(sckPin == 12 && sckPort == my_GPIOB){
		GPIO_WritePin(sckPin, sckPort, afrRegSck, AF7); //set PB12 to AF07 (SPI3_SCK);
	}
	else if(SPIx == my_SPI1 || SPIx == my_SPI2 || SPIx == my_SPI4){
		GPIO_WritePin(sckPin, sckPort, afrRegSck, AF5); //Set to SPI1/2/4_SCK
	}
	else{
		GPIO_WritePin(sckPin, sckPort, afrRegSck, AF6); //Set to SPI3/5_SCK
	}
}



/*
 * @brief	Initializes the MOSI pin for a given SPI peripheral.
 * @param	mosiPin		SPI MOSI Pin
 * @param	mosiPort	Port of the mosi pin
 * @param	SPIx		Target SPI (e.g., my_SPI1)
 */
void SPI_mosiPin_Init(GPIO_Pin_t mosiPin, GPIO_PortName_t mosiPort, SPI_Name_t SPIx){
	GPIO_WritePin(mosiPin, mosiPort, MODER, mode_02); //Config this pin in MODER to Alternate Function mode for SPI interface
	GPIO_Mode_t afrRegMosi = (mosiPin <= 7) ? AFRL : AFRH; //Check which AF reg (AFRL or AFRH) is chosen corresponding to pin number

	/*
	 * SPIx to Alternate Function Mapping (STM32F411VET)
	 * Conditions to check which AFx order is chosen corresponding to mosiPin, mosiPort, and SPIx
	 * SPI3/5 -> AF6 (PD6 special case -> AF5)
	 * SPI1/2/4 -> AF5
	 */
	if(SPIx == my_SPI5 || SPIx == my_SPI3){
		if(mosiPin == my_GPIO_PIN_6 && mosiPort == my_GPIOD){
			GPIO_WritePin(mosiPin, mosiPort, afrRegMosi, AF5);
		}
		else{
			GPIO_WritePin(mosiPin, mosiPort, afrRegMosi, AF6);
		}
	}
	else{
		GPIO_WritePin(mosiPin, mosiPort, afrRegMosi, AF5);
	}
}



/*
 * @brief	Initializes the MISO pin for a given SPI peripheral.
 * @param	misoPin		SPI MISO Pin
 * @param	misoPort	Port of the MISO pin
 * @param	SPIx		Target SPI (e.g., my_SPI1)
 */
void SPI_misoPin_Init(GPIO_Pin_t misoPin, GPIO_PortName_t misoPort, SPI_Name_t SPIx){
	GPIO_WritePin(misoPin, misoPort, MODER, mode_02);
	GPIO_Mode_t afrRegMiso = (misoPin <= 7) ? AFRL : AFRH;

	/*
	 * Conditions to check which AFx order is chosen corresponding to misoPin, misoPort, and SPIx
	 * SPI1/2/4 -> AF5 (PA11 special case -> AF6)
	 * SPI3/5 -> AF6
	 */
	if(SPIx == my_SPI1 || SPIx == my_SPI2 || SPIx == my_SPI4){
		if(misoPin == my_GPIO_PIN_11 && misoPort == my_GPIOA){
			GPIO_WritePin(misoPin, misoPort, afrRegMiso, AF6);
		}
		else{
			GPIO_WritePin(misoPin, misoPort, afrRegMiso, AF5);
		}
	}
	else if(SPIx == my_SPI3 || SPIx == my_SPI5){
		GPIO_WritePin(misoPin, misoPort, afrRegMiso, AF6);
	}
}



/*
 *  @brief	This function configures bit to SPIx's mode
 *
 *  @param	bitPosition		bit location that you want to write
 *  @param	userSPIx		write my_SPI1 when you want to config SPI1
 *  @param	mode			choose specific SPI register to write bit
 *  @param	value			any value that < 32 bits
 */
void WriteSPI(uint8_t bitPosition, SPI_Name_t userSPIx, SPI_Mode_t mode, uint32_t value){
	SPI_Register_Offset_t* SPIx;

	switch(userSPIx){
		case my_SPI1: SPIx = SPI1_REG; break;
		case my_SPI2: SPIx = SPI2_REG; break;
		case my_SPI3: SPIx = SPI3_REG; break;
		case my_SPI4: SPIx = SPI4_REG; break;
		case my_SPI5: SPIx = SPI5_REG; break;
		default: return;
	}

	volatile uint32_t* reg;
	switch(mode){
		case SPI_CR1: reg = &SPIx -> SPI_CR1; break;
		case SPI_CR2: reg = &SPIx -> SPI_CR2; break;
		case SPI_SR: reg = &SPIx -> SPI_SR; break;
		case SPI_DR: reg = &SPIx -> SPI_DR; break;
		case SPI_CRC: reg = &SPIx -> SPI_CRC; break;
		case SPI_RXCRCR: reg = &SPIx -> SPI_RXCRCR; break;
		case SPI_TXCRCR: reg = &SPIx -> SPI_TXCRCR; break;
		case SPI_I2SCFGR: reg = &SPIx -> SPI_I2SCFGR; break;
		case SPI_I2SPR: reg = &SPIx -> SPI_I2SPR; break;
		default: return;
	}

	uint32_t bitWidth = 0;
	uint32_t temp = value;

	//Auto detect bitwidth based on the "value" length
	while(temp > 0){
		bitWidth++; //Increment bitWidth by 1 when temp > 0
		temp = temp >> 1;
	}
	if(value == 0 && bitWidth == 0) bitWidth = 1;
	if(bitPosition + bitWidth > 32) return; //prevent overflow

	//Mask off the old bit and or with the new bit
	uint32_t mask = ((1U << bitWidth) - 1U) << bitPosition;
	uint32_t shiftedValue = (value << bitPosition) & mask;
	*reg = (*reg & ~mask) | shiftedValue;
}
