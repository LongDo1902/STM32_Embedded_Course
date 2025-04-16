/*
 * buttons.h
 *
 *  Created on: Mar 30, 2025
 *      Author: dobao
 */

#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include"stm32f4xx_hal.h"

void buttonB1Init();
char buttonState();
void buttonInterrupt();



#endif /* INC_BUTTONS_H_ */
