/*
 * spi.c
 *
 *  Created on: Jun 11, 2025
 *      Author: dobao
 */

#include "spi.h"


/*
 *	@brief		Reads a single byte of data from an SPI slave device using 8-bit full-duplex SPI
 *				Follows the standard SPI read protocol with a command phase and dummy-byte phase
 *				Works with devices like the L3GD20 or I3G4250 (MSB read = 1).
 *
 *	@param		config		Struct containing SPI peripheral and pin mappings (SPI_GPIO_Config_t)
 *	@param		slaveDeviceAddr		Register address of the target data in the slave device
 *
 *	@return		Received byte from the SPI slave.
 */
char SPI_readReceivedData(SPI_GPIO_Config_t config,
						  char slaveDeviceAddr){
	//Parameters
	const uint8_t READ_FLAG = (1 << 7); //I3G4250 Read Flag (Write 1 to bit 7)
	const uint8_t DUMMYBYTE = 0xFF;

	//Ensure NSS GPIO is clocked and configured as output
//	writePin(config.nssPin, config.nssPort, ODR, my_GPIO_PIN_RESET); //Pull NSS pin low to activate the slave and begin communication
	writePin(config.nssPin, config.nssPort, BSRR, my_GPIO_PIN_RESET); //Pull NSS pin low to activate the slave and begin communication
	for(int i = 0; i < 100; i++); //Add delay when BSRR is used

	while((readSPI(7, config.SPIx, SPI_SR) & 1) == 1); //SPI is busy in communication or TX buffer is not empty
	writeSPI(0, config.SPIx, SPI_DR, (slaveDeviceAddr | READ_FLAG)); //(1 << 7) is refered to L3GD20 gyro for reading mode

	while((readSPI(1, config.SPIx, SPI_SR) & 1) == 0); //Wait until TX buffer is empty
	while((readSPI(7, config.SPIx, SPI_SR) & 1) == 1); //Wait until SPI is not busy
	while((readSPI(0, config.SPIx, SPI_SR) & 1) == 0); //Wait until RX buffer is full data

	char data = readSPI(0, config.SPIx, SPI_DR); //Read and discard first received byte (dummy value received)

	while((readSPI(7, config.SPIx, SPI_SR) & 1) == 1); //Wait until SPI is not busy
	writeSPI(0, config.SPIx, SPI_DR, DUMMYBYTE); //Send dummy byte (0xFF) so the slave can send actual data back

	while((readSPI(1, config.SPIx, SPI_SR) & 1) == 0); //Wait until TX buffer is empty
	while((readSPI(7, config.SPIx, SPI_SR) & 1) == 1); //Wait until SPI is not busy
	while((readSPI(0, config.SPIx, SPI_SR) & 1) == 0); //Wait until RX buffer is full data

	data = readSPI(0, config.SPIx, SPI_DR); //Read actual data
//	writePin(config.nssPin, config.nssPort, ODR, my_GPIO_PIN_SET); //Deactivate the slave, pull NSS pin high again to end communication
	writePin(config.nssPin, config.nssPort, BSRR, my_GPIO_PIN_SET); //Pull NSS pin low to activate the slave and begin communication
	return data;
}



/*
 *
 */
void SPI_write2Device(SPI_GPIO_Config_t config, char slaveDeviceAddr, char writeValue){
	writePin(config.nssPin, config.nssPort, ODR, my_GPIO_PIN_RESET); //Pull NSS pin low to start communicating

	while((readSPI(7, config.SPIx, SPI_SR) & 1) == 1); //SPI is busy in communicating or TX buffer is not yet empty
	writeSPI(0, config.SPIx, SPI_DR, slaveDeviceAddr);

	while((readSPI(1, config.SPIx, SPI_SR) & 1) == 0); //Wait until TX buffer is empty
	while((readSPI(7, config.SPIx, SPI_SR) & 1) == 1); //Wait until SPI is not busy
	while((readSPI(0, config.SPIx, SPI_SR) & 1) == 0); //Wait until RX buffer is full data

	(void)readSPI(0, config.SPIx, SPI_DR); //Read and discard first received byte (dummy value received)

	while((readSPI(7, config.SPIx, SPI_SR) & 1) == 1); //Wait until SPI is not busy
	writeSPI(0, config.SPIx, SPI_DR, writeValue); //Send the value that user want to write to the slave

	while((readSPI(1, config.SPIx, SPI_SR) & 1) == 0); //Wait until TX buffer is empty
	while((readSPI(7, config.SPIx, SPI_SR) & 1) == 1); //Wait until SPI is not busy
	while((readSPI(0, config.SPIx, SPI_SR) & 1) == 0); //Wait until RX buffer is full data

	writePin(config.nssPin, config.nssPort, ODR, my_GPIO_PIN_SET); //Pull the NSS pin high -> deactivate slave
}


/*
 *  @brief	Initializes the selected SPI peripheral and its SCK, MOSI, MISO pins.
 */
void SPI_GPIO_init(SPI_GPIO_Config_t config){

	Enable_GPIO_Clock(config.sckPort);
	Enable_GPIO_Clock(config.nssPort);
	Enable_GPIO_Clock(config.mosiPort);
	Enable_GPIO_Clock(config.misoPort);

	writePin(config.nssPin, config.nssPort, MODER, OUTPUT_MODE); //Set NSS pin as output

	SPI_sckPin_init(config.sckPin, config.sckPort, config.SPIx);
	SPI_mosiPin_init(config.mosiPin, config.mosiPort, config.SPIx);
	SPI_misoPin_init(config.misoPin, config.misoPort, config.SPIx);
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
 * @param	dataFrameSize		Set the data frame size (1 for 16-bits and 0 for 8bits)
 * @param	enableMode			Enable or disable SPI (SPI_Enable_t)
 * @param	baudRateSel			Baud rate prescaler (SPI_BaudRate_t)
 * @param	dataFrameSize		Data frame size for transmission/reception (0 for 8 bits and 1 for 16 bits)
 * @param	softSlaveEn 		Software slave management enable/disable (SPI_SSM_t)
 */
void SPI_basicConfigInit(SPI_GPIO_Config_t config,
						 SPI_MSTR_t masterSlaveSel,
						 SPI_DFF_t dataFrameSize,
						 SPI_BaudRate_t baudRateSel,
						 SPI_SSM_t softSlaveEn,
						 SPI_Enable_t enableMode){

	//Flexible enable SPI clock
	switch(config.SPIx){
		case my_SPI1: __HAL_RCC_SPI1_CLK_ENABLE(); break; //16MHz
		case my_SPI2: __HAL_RCC_SPI2_CLK_ENABLE(); break;
		case my_SPI3: __HAL_RCC_SPI3_CLK_ENABLE(); break;
		case my_SPI4: __HAL_RCC_SPI4_CLK_ENABLE(); break;
		case my_SPI5: __HAL_RCC_SPI5_CLK_ENABLE(); break;
		default: return;
	}

	writeSPI(2, config.SPIx, SPI_CR1, masterSlaveSel); //Set STM32F411VET as master or slave
	writeSPI(11, config.SPIx, SPI_CR1, dataFrameSize); //Set data frame size (must be written before SPI is enabled)
	writeSPI(3, config.SPIx, SPI_CR1, baudRateSel); //Set how fast sckPin can generate (Hz)
	writeSPI(9, config.SPIx, SPI_CR1, softSlaveEn); //Needed if NSS is not physically connected

	/*
	 * bit 8 = 1 -> Simulate NSS being high so that MODF in SR is disabled
	 * bit 8 = 0 -> physical NSS is selected/connected
	 */
	writeSPI(8, config.SPIx, SPI_CR1, softSlaveEn == SOFTWARE_SLAVE_ENABLE ? 1 : 0);

	//Enable SPI must be put after all other features are activated
	writeSPI(6, config.SPIx, SPI_CR1, enableMode); //Disable or Enable SPI
}



/*
 * @brief	Initializes the SCK pin for a given SPI peripheral.
 * @param	sckPin		SPI clock pin
 * @param	sckPort		Port of the SCK pin
 * @param	SPIx		Target SPI (e.g., my_SPI1)
 */
void SPI_sckPin_init(GPIO_Pin_t sckPin, GPIO_PortName_t sckPort, SPI_Name_t SPIx){
	writePin(sckPin, sckPort, MODER, AF_MODE); //Config this pin in MODER to Alternate Function mode for SPI interface
	GPIO_Mode_t afrRegSck = (sckPin <= 7) ? AFRL : AFRH; //Check which AF reg (AFRL or AFRH) is chosen corresponding to pin number

	/*
	 * SPIx to Alternate Function Mapping (STM32F411VET)
	 * Conditions to check which AFx order is chosen corresponding to sckPin, sckPort, and SPIx
	 * SPI1/2/4 -> AF5
	 * SPI3 -> AF6 (PB12 special case -> AF7)
	 * SPI5 -> AF6
	 */
	if(sckPin == 12 && sckPort == my_GPIOB){
		writePin(sckPin, sckPort, afrRegSck, AF7); //set PB12 to AF07 (SPI3_SCK);
	}
	else if(SPIx == my_SPI1 || SPIx == my_SPI2 || SPIx == my_SPI4){
		writePin(sckPin, sckPort, afrRegSck, AF5); //Set to SPI1/2/4_SCK
	}
	else{
		writePin(sckPin, sckPort, afrRegSck, AF6); //Set to SPI3/5_SCK
	}
}



/*
 * @brief	Initializes the MOSI pin for a given SPI peripheral.
 * @param	mosiPin		SPI MOSI Pin
 * @param	mosiPort	Port of the mosi pin
 * @param	SPIx		Target SPI (e.g., my_SPI1)
 */
void SPI_mosiPin_init(GPIO_Pin_t mosiPin, GPIO_PortName_t mosiPort, SPI_Name_t SPIx){
	writePin(mosiPin, mosiPort, MODER, AF_MODE); //Config this pin in MODER to Alternate Function mode for SPI interface
	GPIO_Mode_t afrRegMosi = (mosiPin <= 7) ? AFRL : AFRH; //Check which AF reg (AFRL or AFRH) is chosen corresponding to pin number

	/*
	 * SPIx to Alternate Function Mapping (STM32F411VET)
	 * Conditions to check which AFx order is chosen corresponding to mosiPin, mosiPort, and SPIx
	 * SPI3/5 -> AF6 (PD6 special case -> AF5)
	 * SPI1/2/4 -> AF5
	 */
	if(SPIx == my_SPI5 || SPIx == my_SPI3){
		if(mosiPin == my_GPIO_PIN_6 && mosiPort == my_GPIOD){
			writePin(mosiPin, mosiPort, afrRegMosi, AF5);
		}
		else{
			writePin(mosiPin, mosiPort, afrRegMosi, AF6);
		}
	}
	else{
		writePin(mosiPin, mosiPort, afrRegMosi, AF5);
	}
}



/*
 * @brief	Initializes the MISO pin for a given SPI peripheral.
 * @param	misoPin		SPI MISO Pin
 * @param	misoPort	Port of the MISO pin
 * @param	SPIx		Target SPI (e.g., my_SPI1)
 */
void SPI_misoPin_init(GPIO_Pin_t misoPin, GPIO_PortName_t misoPort, SPI_Name_t SPIx){
	writePin(misoPin, misoPort, MODER, AF_MODE);
	GPIO_Mode_t afrRegMiso = (misoPin <= 7) ? AFRL : AFRH;

	/*
	 * Conditions to check which AFx order is chosen corresponding to misoPin, misoPort, and SPIx
	 * SPI1/2/4 -> AF5 (PA11 special case -> AF6)
	 * SPI3/5 -> AF6
	 */
	if(SPIx == my_SPI1 || SPIx == my_SPI2 || SPIx == my_SPI4){
		if(misoPin == my_GPIO_PIN_11 && misoPort == my_GPIOA){
			writePin(misoPin, misoPort, afrRegMiso, AF6);
		}
		else{
			writePin(misoPin, misoPort, afrRegMiso, AF5);
		}
	}
	else if(SPIx == my_SPI3 || SPIx == my_SPI5){
		writePin(misoPin, misoPort, afrRegMiso, AF6);
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
void writeSPI(uint8_t bitPosition, SPI_Name_t userSPIx, SPI_Mode_t mode, uint32_t value){
	if(bitPosition > 15) return; //Condition to check false entered bitPosition

	volatile SPI_Register_Offset_t* SPIx_p;
	switch(userSPIx){
		case my_SPI1: SPIx_p = SPI1_REG; break;
		case my_SPI2: SPIx_p = SPI2_REG; break;
		case my_SPI3: SPIx_p = SPI3_REG; break;
		case my_SPI4: SPIx_p = SPI4_REG; break;
		case my_SPI5: SPIx_p = SPI5_REG; break;
		default: return;
	}

	volatile uint32_t* reg;

	switch(mode){
		case SPI_CR1: reg = &SPIx_p -> SPI_CR1; break;

		case SPI_CR2:
			if(bitPosition == 3 || bitPosition > 7) return; //These bits are reserved
			reg = &SPIx_p -> SPI_CR2; break;

		case SPI_SR:
			if(bitPosition > 9) return; //These bits are reserved
			reg = &SPIx_p -> SPI_SR; break;

		case SPI_DR: reg = &SPIx_p -> SPI_DR; break;
		case SPI_CRC: reg = &SPIx_p -> SPI_CRC; break;
		case SPI_RXCRCR: reg = &SPIx_p -> SPI_RXCRCR; break;
		case SPI_TXCRCR: reg = &SPIx_p -> SPI_TXCRCR; break;

		case SPI_I2SCFGR:
			if(bitPosition == 6 || bitPosition > 11) return;
			reg = &SPIx_p -> SPI_I2SCFGR; break;

		case SPI_I2SPR:
			if(bitPosition > 9) return;
			reg = &SPIx_p -> SPI_I2SPR; break;

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



/*
 * @brief	Helper function reads specific bit fields or full data from SPI peripheral registers.
 *
 * @param	bitPosition		Bit position or starting bit to read
 * @param	userSPIx		Selected SPI peripheral (my_SPI1 -> my_SPI5)
 * @param	mode			Target SPI register to access (e.g., SPI_CR1, SPI_SR, etc.)
 *
 * @return	Register value or bit field. Returns:
 * 		- 8/16-bits value from SPI_DR based on DFF setting
 * 		- Specific field value (like baud rate or I2S config)
 * 		- 1/0 for single-bit fields
 * 		- (-1) on invalid input or reserved bit access
 */
uint16_t readSPI(uint8_t bitPosition, SPI_Name_t userSPIx, SPI_Mode_t mode){
	const uint16_t ERROR_FLAG = 0xFFFF;

	volatile SPI_Register_Offset_t* SPIx_p;
	switch(userSPIx){
		case my_SPI1: SPIx_p = SPI1_REG; break;
		case my_SPI2: SPIx_p = SPI2_REG; break;
		case my_SPI3: SPIx_p = SPI3_REG; break;
		case my_SPI4: SPIx_p = SPI4_REG; break;
		case my_SPI5: SPIx_p = SPI5_REG; break;
		default: return ERROR_FLAG; //return an error value
	}

	volatile uint32_t* reg;
	switch(mode){
		case SPI_CR1: reg = &SPIx_p -> SPI_CR1; break;

		case SPI_CR2:
			if(bitPosition == 3 || bitPosition > 7) return ERROR_FLAG; //These bits are reserved
			reg = &SPIx_p -> SPI_CR2; break;

		case SPI_SR:
			if(bitPosition > 9) return ERROR_FLAG; //These bits are reserved
			reg = &SPIx_p -> SPI_SR; break;

		case SPI_DR: reg = &SPIx_p -> SPI_DR; break;
		case SPI_CRC: reg = &SPIx_p -> SPI_CRC; break;
		case SPI_RXCRCR: reg = &SPIx_p -> SPI_RXCRCR; break;
		case SPI_TXCRCR: reg = &SPIx_p -> SPI_TXCRCR; break;

		case SPI_I2SCFGR:
			if(bitPosition == 6 || bitPosition > 11) return ERROR_FLAG;
			reg = &SPIx_p -> SPI_I2SCFGR; break;

		case SPI_I2SPR:
			if(bitPosition > 9) return ERROR_FLAG;
			reg = &SPIx_p -> SPI_I2SPR; break;

		default: return ERROR_FLAG;
	}

	/*
	 * Condition to check if the Data Frame is 8 bits or 16 bits
	 */
	uint8_t is16BitFrame = 0; //Default, 8-bits data frame is chosen
	if(((SPIx_p -> SPI_CR1) >> 11) & 1) is16BitFrame = 1; //user choose 16-bits data frame

	if(mode == SPI_DR){
		if(!is16BitFrame) return (uint8_t)(*reg & 0xFF); //Return DR as an 8-bits data
		else return (uint16_t)(*reg & 0xFFFF); //Return DR as an 16-bits data
	}

	// ===== Special bitfield reads =====
	if(mode == SPI_CR1 && bitPosition == 3){ //User want to read baud rate value
		return (*reg >> bitPosition) & 0x7; //0b000000000111
	}

	if(mode == SPI_I2SCFGR && bitPosition == 1){
		return (*reg >> bitPosition) & 0x3; //Read 2-bits DATLEN (data length)
	}

	if(mode == SPI_I2SCFGR && bitPosition == 4){
		return (*reg >> bitPosition) & 0x3; //Read 2 bits I2SSTD
	}

	if(mode == SPI_I2SCFGR && bitPosition == 8){
		return (*reg >> bitPosition) & 0x3; //Read 2bits I2SCFG
	}

	// ===== General 1-bit read =====
	return((*reg >> bitPosition) & 0x1); // return 1 or 0
}

