/*
 * gpio_write_read.h
 *
 *  Created on: Apr 13, 2025
 *      Author: dobao
 */

#ifndef INC_GPIO_WRITE_READ_H_
#define INC_GPIO_WRITE_READ_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "registerAddress.h"

/*
 * Collections of pins 0 to 15
 */
typedef enum{
	my_GPIO_PIN_0 = 0,
	my_GPIO_PIN_1,
	my_GPIO_PIN_2,
	my_GPIO_PIN_3,
	my_GPIO_PIN_4,
	my_GPIO_PIN_5,
	my_GPIO_PIN_6,
	my_GPIO_PIN_7,
	my_GPIO_PIN_8,
	my_GPIO_PIN_9,
	my_GPIO_PIN_10,
	my_GPIO_PIN_11,
	my_GPIO_PIN_12,
	my_GPIO_PIN_13,
	my_GPIO_PIN_14,
	my_GPIO_PIN_15,
}GPIO_Pin_t;

/*
 * Collections of ports from A to H
 */
typedef enum{
	my_GPIOA,
	my_GPIOB,
	my_GPIOC,
	my_GPIOD,
	my_GPIOE,
	my_GPIOH
}GPIO_PortName_t;

/*
 * Collections of different pin's states
 */
typedef enum{
	my_GPIO_PIN_SET = 1, 	//High
	my_GPIO_PIN_RESET = 0,	//Low

	mode_00 = 0x00, //0b00
	mode_01 = 0x01, //0x01
	mode_02 = 0x02, //0b10
	mode_03 = 0x03, //0b11

	//Alternate Function:
	AF0 = 0x0,
	AF1 = 0x1,
	AF2 = 0x2,
	AF3 = 0x3,
	AF4 = 0x4,
	AF5 = 0x5,
	AF6 = 0x6,
	AF7 = 0x7,
	AF8 = 0x8,
	AF9 = 0x9,
	AF10 = 0xA,
	AF11 = 0xB,
	AF12 = 0xC,
	AF13 = 0xD,
	AF14 = 0xE,
	AF15 = 0xF,
}GPIO_State_t;

/*
 * Collections of GPIO Offset Register Name
 */
typedef enum{MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR,
			BSRR, LCKR, AFRL, AFRH}GPIO_Mode_t;

/*
 * Collection of Flash Interface Register Name
 */
typedef enum{FLASH_ACR, FLASH_KEYR, FLASH_OPTKEYR,
			FLASH_SR, FLASH_CR, FLASH_OPTCR} Flash_IntF_Mode_t;


/*
 * Function declarations
 */
void Enable_GPIO_Clock(GPIO_PortName_t port);

void writePin(GPIO_Pin_t pinNum,
					GPIO_PortName_t port,
					GPIO_Mode_t mode,
					GPIO_State_t state);

bool GPIO_LockPin(GPIO_Pin_t pinNum,
				  GPIO_PortName_t port);//HAVE NOT CHECKED YET!!!!!!!!!

char readPin(uint8_t bitPosition,
			GPIO_PortName_t port,
			GPIO_Mode_t mode);

void writeFlash(uint8_t bitPosition,
					Flash_IntF_Mode_t mode,
					uint32_t value);

char readFLASH(uint8_t bitPosition, Flash_IntF_Mode_t mode);


#endif /* INC_GPIO_WRITE_READ_H_ */
