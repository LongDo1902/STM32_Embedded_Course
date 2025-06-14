/*
 * LEDs.h
 *
 *  Created on: Mar 28, 2025
 *      Author: dobao
 */

#ifndef INC_LEDS_H_
#define INC_LEDS_H

#include <stdio.h>
#include "registerAddress.h"
#include "stm32f4xx_hal.h"
#include "gpio_write_read.h"


/*
 * Collections of LED pins which associated with its color
 */
typedef enum{
	LED_Green = 12, //pin12
	LED_Orange,
	LED_Red,
	LED_Blue
}LED_t;

/*
 * Function declaration
 */
void LED_Green_Init();
void LED_Orange_Init();
void LED_Red_Init();
void LED_Blue_Init();

void LED_Control(LED_t LED_pin, int on_off);

#endif /* INC_LEDS_H_ */
