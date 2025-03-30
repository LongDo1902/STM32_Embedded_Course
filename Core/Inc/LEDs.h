/*
 * LEDs.h
 *
 *  Created on: Mar 28, 2025
 *      Author: dobao
 */

#ifndef INC_LEDS_H_
#define INC_LEDS_H_

#include <regBaseAddr.h>
#include "stm32f4xx_hal.h"

#define LED3_PIN 12 //PD12
#define LED4_PIN 13 //PD13
#define LED5_PIN 14 //PD14
#define LED6_PIN 15 //PD15

//Functions Declaration
void LED3_Init();
void LED4_Init();
void LED5_Init();
void LED6_Init();
void LEDs_Init();

void LED_Control(uint8_t pin, int on_off);

#endif /* INC_LEDS_H_ */
