/*
 * i2c.h
 *
 *  Created on: Jul 4, 2025
 *      Author: dobao
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "gpio_write_read.h"
#include "registerAddress.h"
#include "rcc.h"


#define GET_I2C1_REG(mode) (&(I2C1_REG -> mode))
#define GET_I2C2_REG(mode) (&(I2C2_REG -> mode))
#define GET_I2C3_REG(mode) (&(I2C3_REG -> mode))

/*
 * -----------------------------------------
 * Enumeration
 * -----------------------------------------
 */
typedef enum{
	I2C_CR1,
	I2C_CR2,
	I2C_OAR1,
	I2C_OAR2,
	I2C_DR,
	I2C_SR1,
	I2C_SR2,
	I2C_CCR,
	I2C_TRISE,
	I2C_FLTR,

	I2C_REG_COUNT
}I2C_Mode_t;

typedef enum{
	my_I2C1,
	my_I2C2,
	my_I2C3,

	my_I2C_COUNT
}I2C_Name_t;

typedef enum{
	I2C_SM_100K,
	I2C_FM_400K_DUTY_2LOW_1HIGH,
	I2C_FM_400K_DUTY_16LOW_9HIGH,
}I2C_CCR_Mode_t;

typedef enum{
	I2C_OK = 0,
	I2C_ERROR,
	I2C_INVALID_PIN,
	I2C_INVALID_BUS,
	I2C_TIMEOUT,
	I2C_NACK,
	I2C_ACK,
	I2C_BUSY
}I2C_Status_t;

typedef struct{
	I2C_Name_t i2cBus;

	GPIO_Pin_t sclPin;
	GPIO_PortName_t sclPort;

	GPIO_Pin_t sdaPin;
	GPIO_PortName_t sdaPort;
}I2C_GPIO_Config_t;

/*
 * ---------------------------------------------------------
 * Function Declarations
 * ---------------------------------------------------------
 */
void writeI2C(uint8_t bitPosition, I2C_Name_t i2cBus, I2C_Mode_t mode, uint32_t value);
uint32_t readI2C(uint8_t bitPosition, I2C_Name_t i2cBus, I2C_Mode_t mode);
void I2C_basicConfigInit(I2C_GPIO_Config_t config,
						 I2C_CCR_Mode_t mode,
						 uint32_t sclFreq,
						 uint32_t sysClkFreq);

#endif /* INC_I2C_H_ */
