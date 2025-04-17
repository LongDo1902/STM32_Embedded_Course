/*
 * l3gd2.c
 *
 *  Created on: Apr 13, 2025
 *      Author: dobao
 */
#include <stdio.h>
#include "stm32f4xx_hal.h"
#include "l3gd20.h"
#include "gpio_write_read.h"
/*
 * SPI 4-wire mode (MOSI, MISO, CLK, CS)
 * Create helper functions: Write Register and Read Register
 * Write Register: Tell the sensor to save a value at some register
 * Read Register: Ask the sensor to send back the value in a register
 */

/*
 * Helper function
 * Pull CS LOW before sending SPI to tell the sensor you're talking to it
 * Pull CS HIGH after talking to SPI
 */
//#define CS_LOW 0
//#define CS_HIGH 1
//
//void L3GD20_CS_MODE(uint8_t cs_state){ //cs_state: 0 = LOW, cs_state: 1 = HIGH
//	if (cs_state == CS_LOW){
//		GPIO_WritePin(my_GPIOE, my_GPIO_PIN_3, my_GPIO_PIN_RESET); //Set port E pin 3 to low
//	}
//	else if (cs_state == CS_HIGH){
//		GPIO_WritePin(my_GPIOE, my_GPIO_PIN_3, my_GPIO_PIN_SET); //Set port E pin 3 to high
//	}
//}



