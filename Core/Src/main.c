#include <stdio.h>
#include "leds.h"
#include "buttons.h"
#include "registerAddress.h"
#include "gpio_write_read.h"
#include "interrupt.h"
#include "uart.h"

int LED_Delay = 400; //ms
uint32_t* desiredOffsetAddr = (uint32_t*)0x20000000;

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

	LED_Green_Init();
	LED_Red_Init();
	LED_Orange_Init();

	buttonInit(0, my_GPIOA);

	EXTI_TriggerConfig(0, my_EXTI_TRIGGER_BOTH); //Trigger interrupt for rising and falling
	EXTI_Init(0, NVIC_ISER0, EXTI0);
	EXTI_Offset_Init(desiredOffsetAddr);
	user_EXTI_IRQHandler(EXTIFunction, 0x58);

//	UART_Init(6, 7, my_GPIOB, my_UART1, 9600, PARITY_ODD, WORDLENGTH_9B);

	UART_Init(6, 7, my_GPIOB, my_UART1, 9600, PARITY_EVEN, WORDLENGTH_8B);

	while(1){
		my_UART_Transmit(my_UART1, 'b');
		if (my_UART_Receive(my_UART1) == 'a'){
			LED_Control(LED_Red, 1);
		} else{
			LED_Control(LED_Red, 0);
		}

//		my_UART_Transmit(my_UART1, 'a');
//		LED_Control(LED_Red, 1);
//		HAL_Delay(LED_Delay);
//		LED_Control(LED_Red, 0);
//		HAL_Delay(LED_Delay);
	}
}
