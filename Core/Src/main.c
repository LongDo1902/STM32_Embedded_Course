#include <stdio.h>
#include "LEDs.h"
#include "buttons.h"
#include "registerAddress.h"
#include "gpio_write_read.h"
#include "interrupt.h"
#include "uart.h"

int LED_Delay = 300; //ms

void EXTIFunction(){
	if(buttonState()){
		LED_Control(LED_Green, 1);
	}
	else{
		LED_Control(LED_Green, 0);
	}
	GPIO_WriteEXTI(0, PR, my_GPIO_PIN_SET); //Clear the flag
}


int main(void){
	HAL_Init();
	EXTI_TriggerConfig(0, my_EXTI_TRIGGER_BOTH); //Trigger interrupt for rising and falling
	EXTI_Init(0, NVIC_ISER0, EXTI0);
	EXTI_Offset_Init(0x20000000);
	user_EXTI_IRQHandler(EXTIFunction, 0x58);
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
