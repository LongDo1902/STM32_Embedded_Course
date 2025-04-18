#include <stdio.h>
#include "LEDs.h"
#include "buttons.h"
#include "registerAddress.h"
#include "gpio_write_read.h"

int LED_Delay = 300; //ms

void function(){
	if(buttonState()){
		LED_Control(LED_Green, 1);
	}
	else{
		LED_Control(LED_Green, 0);
	}
	EXTI_REG -> PR = (1 << 0);
}


int main(void){
	HAL_Init();
	EXTI_Init(0, NVIC_ISER0, EXTI0);
	EXTI_Offset_Init();
	user_EXTI_IRQHandler(function);
	LED_Green_Init();
	LED_Red_Init();
	buttonInit(0, my_GPIOA);

	while(1){
		LED_Control(LED_Red, 1);
		HAL_Delay(LED_Delay);
		LED_Control(LED_Red, 0);
		HAL_Delay(LED_Delay);
	}
}







