/*
 * @file	i2c.c
 *
 *  Created on: Jul 4, 2025
 *      Author: dobao
 *
 *	The module provides:
 *		Register lookup tables for I2C1-3 (avoids repetitive switch-cases)
 *		Mask tables that mark *reserved* bits so we never write them by mistake
 *		Tiny read/write helpers that perform field-sized RMW operations
 *		Pin initialization helpers for every legal SCL/SDA mapping on STM32F411 including correct pull-up handling
 */
#include "i2c.h"

/*
 * -----------------------------------------------------------------
 * Private Helpers
 * -----------------------------------------------------------------
 */

/*
 * @brief	Bit-mask of **writable** bits for every I2C reg
 * 			A clear bit (0) marks a *reserved* position that **must not** be written.
 *
 * 			Index:  ::I2C_Mode_t
 */
static const uint32_t I2C_VALID_BITS[I2C_REG_COUNT] = {
		[I2C_CR1] = ~((1u << 2) | (1u << 14)),
		[I2C_CR2] = ~((0b11 << 6) | (0b111 << 13)),
		[I2C_OAR1] = ~(0b11111 << 10),
		[I2C_OAR2] = ~(0xFF << 8),
		[I2C_DR] = ~(0xFF << 8),
		[I2C_SR1] = ~((1u << 5) | (1u << 13)),
		[I2C_SR2] = ~(1u << 3),
		[I2C_CCR] = ~(0b11 << 12),
		[I2C_TRISE] = ~(0x3FF << 6),
		[I2C_FLTR] = ~(0x7FF << 5),
};

/*
 * @brief	Lookup table for the I2C1 peripheral register
 *
 * 			This static pointer array maps each value of ::I2C_Mode_t (index) to memory-mapped address
 * 			of the corresponding I2C1 Register. Using the table avoids open-coded 'switch'/'if' blocks
 * 			and makes register access simply
 *
 * 			**I2C_REG_COUNT**	Array Length
 */
#define REG_TABLE_ATTR static volatile uint32_t* const

REG_TABLE_ATTR I2C1RegLookupTable[I2C_REG_COUNT] = {
		[I2C_CR1] = GET_I2C1_REG(I2C_CR1),
		[I2C_CR2] = GET_I2C1_REG(I2C_CR2),
		[I2C_OAR1] = GET_I2C1_REG(I2C_OAR1),
		[I2C_OAR2] = GET_I2C1_REG(I2C_OAR2),
		[I2C_DR] = GET_I2C1_REG(I2C_DR),
		[I2C_SR1] = GET_I2C1_REG(I2C_SR1),
		[I2C_SR2] = GET_I2C1_REG(I2C_SR2),
		[I2C_CCR] = GET_I2C1_REG(I2C_CCR),
		[I2C_TRISE] = GET_I2C1_REG(I2C_TRISE),
		[I2C_FLTR] = GET_I2C1_REG(I2C_FLTR),
};

REG_TABLE_ATTR I2C2RegLookupTable[I2C_REG_COUNT] = {
		[I2C_CR1] = GET_I2C2_REG(I2C_CR1),
		[I2C_CR2] = GET_I2C2_REG(I2C_CR2),
		[I2C_OAR1] = GET_I2C2_REG(I2C_OAR1),
		[I2C_OAR2] = GET_I2C2_REG(I2C_OAR2),
		[I2C_DR] = GET_I2C2_REG(I2C_DR),
		[I2C_SR1] = GET_I2C2_REG(I2C_SR1),
		[I2C_SR2] = GET_I2C2_REG(I2C_SR2),
		[I2C_CCR] = GET_I2C2_REG(I2C_CCR),
		[I2C_TRISE] = GET_I2C2_REG(I2C_TRISE),
		[I2C_FLTR] = GET_I2C2_REG(I2C_FLTR),
};

REG_TABLE_ATTR I2C3RegLookupTable[I2C_REG_COUNT] = {
		[I2C_CR1] = GET_I2C3_REG(I2C_CR1),
		[I2C_CR2] = GET_I2C3_REG(I2C_CR2),
		[I2C_OAR1] = GET_I2C3_REG(I2C_OAR1),
		[I2C_OAR2] = GET_I2C3_REG(I2C_OAR2),
		[I2C_DR] = GET_I2C3_REG(I2C_DR),
		[I2C_SR1] = GET_I2C3_REG(I2C_SR1),
		[I2C_SR2] = GET_I2C3_REG(I2C_SR2),
		[I2C_CCR] = GET_I2C3_REG(I2C_CCR),
		[I2C_TRISE] = GET_I2C3_REG(I2C_TRISE),
		[I2C_FLTR] = GET_I2C3_REG(I2C_FLTR),
};


/*
 * -----------------------------------------------------------------
 * Bit-manipulation Helpers
 * -----------------------------------------------------------------
 */

/*
 * @brief	Check that 'mode' is in range and 'bitPosition' is not reserved
 * 			This function flexibly check if multi-bits are valid
 *
 * @return	true	Inserted bit indice is valid
 * 			false	...invalid or reserved bit
 */
static inline bool isValidI2CBit(uint8_t bitPosition, uint8_t bitWidth, I2C_Mode_t mode){
	if(mode >= I2C_REG_COUNT || bitWidth == 0 || ((bitPosition + bitPosition) > 32)) return false;
	uint32_t mask = ((bitWidth == 32) ? 0xFFFFFFFFu : ((1U << bitWidth) - 1U) << bitPosition);

	return (I2C_VALID_BITS[mode] & mask) == mask;
}

/*
 * @brief	Generic masked write helper
 *
 * @param	reg		Pointers to the register
 * @param	bitPosition		First bit of the field
 * @param	bitWidth		Field width in bits
 * @param	value			Field value (must fit in @p bitWidth)
 */
static void writeI2CBits(volatile uint32_t* reg, uint8_t bitPosition, uint8_t bitWidth, uint32_t value){
	/*
	 * The function leaves the register unchanged if
	 * 		@p bitPosition larger than 31 because shifting by 32 is undefined in C
	 * 		@p value is too large for the field
	 * 		The field would spill past bit 31
	 */
	if(bitPosition > 31 || bitWidth > 32) return;
	if(bitWidth < 32 && value >= (1U << bitWidth)) return;
	if((bitWidth + bitPosition) > 32) return;

	//Mask off the old bit and OR with the new value
	uint32_t mask = (bitWidth == 32) ? 0xFFFFFFFFu : ((1U << bitWidth) - 1) << bitPosition;
	uint32_t shiftedValue = (value << bitPosition);
	*reg = (*reg & ~mask) | (shiftedValue & mask);
}


/*
 * @brief	Read a field of "bitWidth" bits from a register starting at 'bitPosition'
 *
 * @param	reg (pointer) to the register
 * @param	bitPosition		Starting bit position (0-31)
 * @param	bitWidth		Number of bits/bit size that fit @p value
 */
static uint32_t readI2CBits(volatile uint32_t* reg, uint8_t bitPosition, uint8_t bitWidth){
	if(bitWidth == 32) return *reg; //Full-word: no mask needed

	uint32_t mask = ((1U << bitWidth) - 1U);
	return (*reg >> bitPosition) & mask;
}


/*
 * -----------------------------------------------------------------
 * Clock Helpers
 * -----------------------------------------------------------------
 */

/*
 * @brief	Enable GPIOs' clock
 */
static void enableGPIOClock(GPIO_PortName_t port){
	switch(port){
		case my_GPIOA: my_RCC_GPIOA_CLK_ENABLE(); break;
		case my_GPIOB: my_RCC_GPIOB_CLK_ENABLE(); break;
		case my_GPIOC: my_RCC_GPIOC_CLK_ENABLE(); break;
		case my_GPIOD: my_RCC_GPIOD_CLK_ENABLE(); break;
		case my_GPIOE: my_RCC_GPIOE_CLK_ENABLE(); break;
		case my_GPIOH: my_RCC_GPIOH_CLK_ENABLE(); break;
		default: return;
	}
}


/*
 * -----------------------------------------------------------------
 * Pin Initialization Helpers (SCL and SDA)
 * -----------------------------------------------------------------
 */

/*
 * @brief	A helper function to intialize SCL pin for the selected I2C peripheral
 *
 * The function performs:
 * 		1. Validation of <pin, port> against the requested bus
 * 		2. GPIO setup:
 * 			Alternate function mode
 * 			Open-Drain mode
 * 			Very-high speed
 * 		3. Pull-up handling
 * 			PB6 already has a 4.7kOhm external pull-up resistor
 * 			Use internal pull-up resistor for the rest of the pins
 */
static I2C_Status_t I2C_sclPinInit(GPIO_Pin_t sclPin,
								   GPIO_PortName_t sclPort,
								   I2C_Name_t i2cBus){
	/* Validate pin, port, and bus first */
	switch(i2cBus){
		case my_I2C1:
			if(!(sclPort == my_GPIOB && (sclPin == my_GPIO_PIN_6 || sclPin == my_GPIO_PIN_8))){
				return I2C_INVALID_PIN;
			}
			break;

		case my_I2C2:
			if(!(sclPort == my_GPIOB && sclPin == my_GPIO_PIN_10)){
				return I2C_INVALID_PIN;
			}
			break;

		case my_I2C3:
			if(!(sclPort == my_GPIOA && sclPin == my_GPIO_PIN_8)){
				return I2C_INVALID_PIN;
			}
			break;

		default: return I2C_INVALID_BUS;
	}

	/* Config GPIO for I2C purpose */
	enableGPIOClock(sclPort);
	const GPIO_Mode_t afrReg = (sclPin < my_GPIO_PIN_8) ? AFRL : AFRH;
	const bool hasExtPullUp = (sclPin == my_GPIO_PIN_6 && sclPort == my_GPIOB);

	writePin(sclPin, sclPort, MODER, AF_MODE); //Set pin to Alternate Function Mode
	writePin(sclPin, sclPort, OTYPER, OPEN_DRAIN); //Set pin to Open-Drain mode
	writePin(sclPin, sclPort, OSPEEDR, HIGH_SPEED); //Set a very-high speed output pin
	writePin(sclPin, sclPort, PUPDR, hasExtPullUp ? FLOATING : PULL_UP);
	writePin(sclPin, sclPort, afrReg, AF4); //SCL pins use AF4 on all valid pins of this MCU

	return I2C_OK;
}


/*
 * @brief	Configure the **SDA** pin for the selected I2C peripheral.
 *
 * The function performs:
 * 		1. Validation of <pin, port> against the requested bus
 * 		2. GPIO setup:
 * 			Alternate function mode
 * 			Open-Drain mode
 * 			Very-high speed
 * 		3. Pull-up handling
 * 			PB9 already has a 4.7kOhm external pull-up resistor
 * 			Use internal pull-up resistor for the rest of the pins
 * 		4. AF9 handling for some SDA pins from I2C2 and I2C3 peripherals
 */
static I2C_Status_t I2C_sdaPinInit(GPIO_Pin_t sdaPin, GPIO_PortName_t sdaPort, I2C_Name_t i2cBus){
	uint8_t alternateFuncMode = 0xFF;

	switch(i2cBus){
		case my_I2C1:
			if((sdaPort == my_GPIOB && (sdaPin == my_GPIO_PIN_7 || sdaPin == my_GPIO_PIN_9))){
				alternateFuncMode = AF4;
			} else return I2C_INVALID_PIN;
			break;

		case my_I2C2:
			if(sdaPort == my_GPIOB && sdaPin == my_GPIO_PIN_11){
				alternateFuncMode = AF4;
			}
			else if(sdaPort == my_GPIOB && sdaPin == my_GPIO_PIN_3){
				alternateFuncMode = AF9;
			}
			else return I2C_INVALID_PIN;
			break;

		case my_I2C3:
			if(sdaPort == my_GPIOC && sdaPin == my_GPIO_PIN_9){
				alternateFuncMode = AF4;
			}
			else if((sdaPin == my_GPIO_PIN_4 && sdaPort == my_GPIOB) ||
					(sdaPin == my_GPIO_PIN_8 && sdaPort == my_GPIOB)){
				alternateFuncMode = AF9;
			}
			else return I2C_INVALID_PIN;
			break;

		default: return I2C_INVALID_BUS;
	}

	/* Config GPIO for I2C purpose*/
	enableGPIOClock(sdaPort);
	const GPIO_Mode_t afrReg = (sdaPin < my_GPIO_PIN_8) ? AFRL : AFRH;
	const bool hasExtPullUp = (sdaPort == my_GPIOB && sdaPin == my_GPIO_PIN_9);

	writePin(sdaPin, sdaPort, MODER, AF_MODE); //Set pin to Alternate Function Mode
	writePin(sdaPin, sdaPort, OTYPER, OPEN_DRAIN); //Set pin to Open-Drain mode
	writePin(sdaPin, sdaPort, OSPEEDR, HIGH_SPEED); //Set a very-high speed output pin
	writePin(sdaPin, sdaPort, PUPDR, hasExtPullUp ? FLOATING : PULL_UP); //0b00 = floating, 0b01 = internal pull-up resistor
	writePin(sdaPin, sdaPort, afrReg, alternateFuncMode);

	return I2C_OK;
}


/*
 * @brief	Intialize SCL and SDA pin of selected I2C peripheral
 *
 * @return	I2C_OK on success
 * 			I2C_INVALID_BUS on illegal peripheral index.
 */
static I2C_Status_t I2C_GPIO_init(I2C_GPIO_Config_t config){
	if(config.i2cBus >= my_I2C_COUNT){
		return I2C_INVALID_BUS;
	}
	/* Configure GPIOs */
	I2C_sclPinInit(config.sclPin, config.sclPort, config.i2cBus);
	I2C_sdaPinInit(config.sdaPin, config.sdaPort, config.i2cBus);
	return I2C_OK;
}


/*
 * @brief	Calculate the value that must be written into I2C_CCR register so the bus
 * 			clock (SCL) runs at the request speed, then write that value - together with
 * 			Fast/Standard mode and duty cycle bits to the hardware register
 *
 * @param	mode	::I2C_CCR_Mode_t
 * 					I2C_SM_100K: Standard-mode 100KHz
 * 					I2C_FM_400K_DUTY_2LOW_1HIGH: Fast-mode 400kHz, Tlow/Thigh = 2/1
 * 					I2C_FM_400k_DUTY_16LOW_9HIGH: Fast-mode 400kHz, TLow/Thigh = 16/9
 *
 * @param	sclFreq		Desired SCL frequency in hertz (100k or 400k)
 * @param	sysClkFreq	Peripheral Clock that feeds the I2C unit
 * @param 	config		Structure that hold the I2C pins and i2cBus
 *
 * @retval	uint32_t crr value (0 to 0x0FFF) on success
 * 			ERROR if something wrong.
 */
static I2C_Status_t I2C_getCCR(I2C_CCR_Mode_t mode,
					   uint32_t sclFreq,
					   uint32_t sysClkFreq,
					   I2C_GPIO_Config_t config){

	/*
	 * sclFreq must be non-zero and not greater than 400kHz
	 * sysClkFreq must be at least 2MHz
	 */
	if(sclFreq == 0 || sclFreq > 400000U || sysClkFreq == 0 || sysClkFreq < 2000000U) return I2C_ERROR;

	uint32_t minCcr = 4;
	uint32_t ccr = 0;
	uint8_t fsMode = 0;
	uint8_t dutyMode = 0;

	switch(mode){
		case I2C_SM_100K:
			fsMode = 0;
			dutyMode = 0;
			ccr = sysClkFreq / (2U * sclFreq);
			break;

		case I2C_FM_400K_DUTY_2LOW_1HIGH:
			fsMode = 1;
			dutyMode = 0;
			ccr = sysClkFreq / (3U * sclFreq);
			minCcr = 1; //Fast mode allows minimum CCR value = 1
			break;

		case I2C_FM_400K_DUTY_16LOW_9HIGH:
			fsMode = 1;
			dutyMode = 1;
			ccr = sysClkFreq / (25U * sclFreq);
			minCcr = 1; //Fast mode allows minimum CCR value = 1
			break;

		default: return I2C_ERROR;
	}
	if(ccr < minCcr) ccr = minCcr;
	if(ccr > 0x0FFF) return I2C_ERROR;

	/* Program bit 14 and 15 of I2C_CCR and write CCR val into I2C_CCR */
	writeI2C(15, config.i2cBus, I2C_CCR, fsMode);
	writeI2C(14, config.i2cBus, I2C_CCR, dutyMode);
	writeI2C(0, config.i2cBus, I2C_CCR, ccr);

	return I2C_OK;
}


/*
 * --------------------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------------------
 */

/*
 * @brief	write one byte to a register of a 7-bit addressed I2C slave
 *
 * Sequence:
 * 		1. Wait until the bus is idle.
 * 		2. Generate a START
 * 		3. Send <slaveAddr, Write>.
 * 		4. Abort on a NACK (AF flag)
 * 		5. Send the target register address
 * 		6. send the data byte
 * 		7. Generate a STOP
 *
 * @param	config			::I2C_GPIO_Config_t, config.i2cBus (my_I2C1 to my_I2C3)
 * @param	slaveAddr		Slave device address which is a 7-bits address
 * @param	slaveRegAddr	Desired reg addr of that slave device that we want to write the value in
 * @param	value			Single Byte Data packet is ready to be sent from master to slave device.
 */
I2C_Status_t I2C_singleByteWrite(I2C_GPIO_Config_t config, uint8_t slaveAddr, uint8_t slaveRegAddr, uint8_t value){
	while((readI2C(1, config.i2cBus, I2C_SR2) & 1u) == 1u); //Wait until bus is not busy

	/* Start a transaction */
	writeI2C(8, config.i2cBus, I2C_CR1, SET); //1: Start generation
	while((readI2C(0, config.i2cBus, I2C_SR1) & 1u) == 0u); //Wait until start condition generated

	/* Send the 7-bit slave address + write bit */
	uint8_t addrByte = (slaveAddr << 1) | 0; //Offset slave addr to start at bit 1 and end at 7 and leave bitPos 0 = 0 which indicates write mode
	writeI2C(0, config.i2cBus, I2C_DR, addrByte); //Write the slave addr + write mode indicator to DR holder
	while((readI2C(1, config.i2cBus, I2C_SR1) & 1u) == 0u); //Wait until the slave's address is sent

	/* Read SR1 and SR2 to clear the bit ADDR in SR1 */
	(void)readI2C(0, config.i2cBus, I2C_SR1); //Dummy read
	(void)readI2C(0, config.i2cBus, I2C_SR2); //Dummy read

	if((readI2C(10, config.i2cBus, I2C_SR1) & 1u) == 1u){ //AF?
		writeI2C(9, config.i2cBus, I2C_CR1, SET); //STOP
		return I2C_NACK;
	}

	while((readI2C(10, config.i2cBus, I2C_SR1) & 1u) == 1u); //Wait until there is ACK signal from slave

	/* Send the internal slave's register address (command byte) */
	while((readI2C(7, config.i2cBus, I2C_SR1) & 1u) == 0u); //Wait until data register (transmitters) is empty
	writeI2C(0, config.i2cBus, I2C_DR, slaveRegAddr);
	while((readI2C(2, config.i2cBus, I2C_SR1) & 1u) == 0u); //Wait until data byte transfer succeeded
	while((readI2C(10, config.i2cBus, I2C_SR1) & 1u) == 1u); //Wait until there is ACK signal from slave

	/* Send the data byte / value to internal slave reg addr */
	writeI2C(0, config.i2cBus, I2C_DR, value);
	while((readI2C(2, config.i2cBus, I2C_SR1) & 1u) == 0u); //Wait until data byte transfer succeeded

	/* Generate stop bit */
	writeI2C(9, config.i2cBus, I2C_CR1, SET);

	return I2C_OK;
}


/*
 * @brief	Initialize basic configurations for I2C
 *
 * 			Enable clock for selected I2C bus
 * 			Then, initialize I2C-related pins
 * 			Must select APB frequency that matches setting in RCC.c which is 50MHz
 * 			Have a helper function (I2C_getCCR) that get automatically get CCR value based on sysClkFreq, desired SCL clock and Sm or Fm mode
 * 			Set TRISE to let MCU know the time limit of getting from low to high so it can keep the timing orrect
 * 			Enable I2C peripheral
 *
 * @param 	config	Pin mapping and bus identifier for this I²C instance.
 * @param	mode	Timing profile (100kHz standard mode or one of the 400kHz fast-mode options).
 * @param	sclFreq 	Desired SCL clock in hertz (100000 or 400000).
 * @param	sysClkFreq 	Peripheral clock frequency driving the I²C hardware, in hertz.
 */
void I2C_basicConfigInit(I2C_GPIO_Config_t config,
						 I2C_CCR_Mode_t mode,
						 uint32_t sclFreq,
						 uint32_t sysClkFreq){
	//Flexible enable I2C clock
	switch(config.i2cBus){
		case my_I2C1: my_RCC_I2C1_CLK_ENABLE(); break;
		case my_I2C2: my_RCC_I2C2_CLK_ENABLE(); break;
		case my_I2C3: my_RCC_I2C3_CLK_ENABLE(); break;
		default: return;
	}
	I2C_GPIO_init(config);
	writeI2C(0, config.i2cBus, I2C_CR1, RESET); //Disable I2C peripheral before configuring it
	writeI2C(0, config.i2cBus, I2C_CR2, (sysClkFreq/1000000U)); //Set this I2C's clock freq to 50MHz
	if(I2C_getCCR(mode, sclFreq, sysClkFreq, config) != I2C_OK) return;
//	writeI2C(0, config.i2cBus, I2C_TRISE, 51); //1000ns(from I2C spec), 20ns from 50MHz (TRISE = 1 + (1000/20) = 51)
	writeI2C(0, config.i2cBus, I2C_CR1, SET); //Enable I2C peripheral
}


/*
 * @brief	Write a bit-field to an I2C peripheral register
 *
 * 			Figures out how many bits the setting needs
 * 			Won't touch ay bits the datasheet says are off-limits.
 * 			Changes only the bits you asked for, leaving the rest unchanged.
 * 			Does nothing at all if you pass a bad argument or pick an I2C unit that does not exit
 *
 * @param	bitPosition		The LSB index of the field (0-31)
 * @param	i2cBus			Which peripheral to talk to (my_I2C1, my_I2C2, my_I2C3)
 * @param	mode			Which register of I2C to access (enum @ref I2C_Mode_t)
 *
 * @param	value			The value to write. There is a helper function to check if
 * 							@p value fits into the target field width; if it does not,
 * 							the call ignore
 */
void writeI2C(uint8_t bitPosition, I2C_Name_t i2cBus, I2C_Mode_t mode, uint32_t value){
	//Early sanity
	uint8_t bitWidth = 1;

	switch(mode){
		case I2C_CR2:
			if(bitPosition == 0) bitWidth = 6;
			break;

		case I2C_OAR1:
			if(bitPosition == 1) bitWidth = 7;
			else if(bitPosition == 8) bitWidth = 2;
			break;

		case I2C_OAR2:
			if(bitPosition == 1) bitWidth = 7;
			break;

		case I2C_DR:
			bitWidth = 8;
			break;

		case I2C_SR2:
			if(bitPosition == 8) bitWidth = 8;
			break;

		case I2C_CCR:
			if(bitPosition == 0) bitWidth = 12;
			break;

		case I2C_TRISE:
			bitWidth = 6;
			break;

		case I2C_FLTR:
			if(bitPosition == 0) bitWidth = 0;
			break;

		default: return;
	}

	if(!isValidI2CBit(bitPosition, bitWidth, mode)) return;

	/* Map bus -> Register pointer	 */
	volatile uint32_t* reg = NULL;

	switch(i2cBus){
		case my_I2C1:
			reg = I2C1RegLookupTable[mode];
			break;

		case my_I2C2:
			reg = I2C2RegLookupTable[mode];
			break;

		case my_I2C3:
			reg = I2C3RegLookupTable[mode];
			break;

		default: return;
	}
	if(reg == NULL) return; //Peripheral not present on this part

	//Disallow writes to read-only status reg except write to clear bits
	if((mode == I2C_SR1 || mode == I2C_SR2) && value != 0){
		//Only allow write-to-clear its (set value to 0 to clear)
			return;
	}

	writeI2CBits(reg, bitPosition, bitWidth, value);
}


/*
 * @brief	Read a bit-field from an I2C peripheral register
 *
 * @param	bitPosition		The LSB index of the field (0-31)
 * @param	i2cBus			Which peripheral to talk to (my_I2C1, my_I2C2, my_I2C3)
 * @param	mode			Which register of I2C to access (enum @ref I2C_Mode_t)
 *
 * @param	value			The value to write. There is a helper function to check if
 * 							@p value fits into the target field width; if it does not,
 * 							the call ignore
 *
 * @return	The extracted field value on success.
 * 			If the cal is invalid, the constant @c 0xFFFFFFFF is returned as an ERROR_FLAG
 */
uint32_t readI2C(uint8_t bitPosition, I2C_Name_t i2cBus, I2C_Mode_t mode){
	uint32_t const ERROR_FLAG = 0xFFFFFFFF;
	uint8_t bitWidth = 1;

	switch(mode){
		case I2C_CR2:
			if(bitPosition == 0) bitWidth = 6;
			break;

		case I2C_OAR1:
			if(bitPosition == 1) bitWidth = 7;
			else if(bitPosition == 8) bitWidth = 2;
			break;

		case I2C_OAR2:
			if(bitPosition == 1) bitWidth = 7;
			break;

		case I2C_DR:
			bitWidth = 8;
			break;

		case I2C_SR2:
			if(bitPosition == 8) bitWidth = 8;
			break;

		case I2C_CCR:
			if(bitPosition == 0) bitWidth = 12;
			break;

		case I2C_TRISE:
			bitWidth = 6;
			break;

		case I2C_FLTR:
			if(bitPosition == 0) bitWidth = 0;
			break;

		default: return ERROR_FLAG;
	}

	if(!isValidI2CBit(bitPosition, bitWidth, mode)) return ERROR_FLAG;

	volatile uint32_t* reg = NULL;

	switch(i2cBus){
		case my_I2C1:
			reg = I2C1RegLookupTable[mode];
			break;

		case my_I2C2:
			reg = I2C2RegLookupTable[mode];
			break;

		case my_I2C3:
			reg = I2C3RegLookupTable[mode];
			break;

		default: return ERROR_FLAG;
	}

	if(reg == NULL) return ERROR_FLAG;

	return readI2CBits(reg, bitPosition, bitWidth);
}





