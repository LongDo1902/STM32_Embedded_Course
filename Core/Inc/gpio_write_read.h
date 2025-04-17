/*
 * gpio_write_read.h
 *
 *  Created on: Apr 13, 2025
 *      Author: dobao
 */

#ifndef INC_GPIO_WRITE_READ_H_
#define INC_GPIO_WRITE_READ_H_
#include <stdint.h>

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
	my_GPIO_PIN_15
}GPIO_pin_t;

typedef enum{
	my_GPIOA,
	my_GPIOB,
	my_GPIOC,
	my_GPIOD,
	my_GPIOE,
	my_GPIOH
} GPIO_portName_t;


typedef enum{MODER, OTYPER, OSPEEDR, PUPDR, BSRR}GPIO_Mode_t;


typedef enum{
	my_GPIO_PIN_SET = 1,
	my_GPIO_PIN_RESET = 0,
	mode_0x00 = 0x00, //0b00
	mode_0x01 = 0x01, //0x01
	mode_0x02 = 0x02, //0b10
	mode_0x03 = 0x03  //0b11
}GPIO_State_t;

/*
 * Function declarations
 */
void GPIO_WritePin (GPIO_pin_t pinNum,
					GPIO_portName_t port,
					GPIO_State_t state,
					GPIO_Mode_t mode);


#endif /* INC_GPIO_WRITE_READ_H_ */
