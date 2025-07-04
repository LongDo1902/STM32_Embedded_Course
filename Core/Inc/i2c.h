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
	my_I2C3
}I2C_Name_t;

#endif /* INC_I2C_H_ */
