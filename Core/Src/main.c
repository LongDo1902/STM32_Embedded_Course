#include <stdio.h>
#include "LEDs.h"
#include "buttons.h"
#include "registerAddress.h"
#include "interrupts.h"

int LED_Delay = 300; //ms


int main(void){
	HAL_Init();
	EXTI0_Init();
	offsetVectorTable();
	redirect_EXTI0_IRQHandler();
	LED_Green_Init();
	LED_Red_Init();
	buttonB1Init();

	while(1){
		LED_Control(LED_Red, 1);
		HAL_Delay(LED_Delay);
		LED_Control(LED_Red, 0);
		HAL_Delay(LED_Delay);
	}
}







