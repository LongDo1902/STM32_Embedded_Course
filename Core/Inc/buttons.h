/*
 * buttons.h
 *
 *  Created on: Mar 30, 2025
 *      Author: dobao
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include "stm32f4xx_hal.h"
#include "gpio_write_read.h"

void buttonInit(char buttonPin, GPIO_portName_t portName);
char buttonState();
void buttonInterrupt();



#endif /* INC_BUTTONS_H_ */
