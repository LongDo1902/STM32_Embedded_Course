#include <stdio.h>
#include "LEDs.h"
#include "buttons.h"

int LED_Delay = 50;

int main(void){
	HAL_Init();
	LEDs_Init();
	buttonB1Init();

	while(1){
		buttonControl();
//		LED_Control(LED3_PIN, 1); //Turn on
//		HAL_Delay(LED_Delay); //ms
//		LED_Control(LED3_PIN, 0); //Turn on
//		HAL_Delay(LED_Delay); //ms
//
//		LED_Control(LED4_PIN, 1);
//		HAL_Delay(LED_Delay); //ms
//		LED_Control(LED4_PIN, 0);
//		HAL_Delay(LED_Delay); //ms
//
//		LED_Control(LED5_PIN, 1);
//		HAL_Delay(LED_Delay); //ms
//		LED_Control(LED5_PIN, 0);
//		HAL_Delay(LED_Delay); //ms
//
//		LED_Control(LED6_PIN, 1);
//		HAL_Delay(LED_Delay); //ms
//		LED_Control(LED6_PIN, 0);
//		HAL_Delay(LED_Delay); //ms
	}
}
