#include <stdio.h>
#include <string.h>

#include "stm32f4xx_hal.h"
#include "leds.h"
#include "buttons.h"
#include "registerAddress.h"
#include "gpio_write_read.h"
#include "uart.h"
#include "spi.h"
#include "timer.h"
#include "exti.h"
#include "rcc.h"


char session[15] = "RCC";
int userDelay = 2000; //ms

uint32_t* desiredOffsetAddr = (uint32_t*)0x20000000;


void userIRQHandlerFunction(){ // 0x0800085c
	if(buttonState()){
		LED_Control(LED_Green, 1);
	}
	else{
		LED_Control(LED_Green, 0);
	}
	writeEXTI(0, PR, SET); //Clear the flag
}


int main(void){
	initTimer(my_TIM1);

	if(strcmp(session, "EXTI") == 0){
		buttonInit(0, my_GPIOA);
		LED_Red_Init();
		LED_Green_Init();

		EXTI_init(0, my_EXTI_TRIGGER_BOTH, EXTI0);
		vectorTableOffset(desiredOffsetAddr);
		user_IRQHandler(userIRQHandlerFunction, 0x58); //How to do this??? The green interrupt does not work here
		while(1){
			LED_Control(LED_Red, 1);
			delay(userDelay);
			LED_Control(LED_Red, 0);
			delay(userDelay);
		}
	}

	else if(strcmp(session, "UART") == 0){
		LED_Red_Init();
		UART_Init(my_GPIO_PIN_6, my_GPIO_PIN_7, my_GPIOB, my_UART1, 9600, PARITY_ODD, WORDLENGTH_9B);
		while(1){
			my_UART_Transmit(my_UART1, 'a');

			if(my_UART_Receive(my_UART1) == 'g'){
				LED_Control(LED_Red, 1);
			}
			else{
				LED_Control(LED_Red, 0);
			}
		}
	}

	else if(strcmp(session, "SPI") == 0){

		SPI_GPIO_Config_t spiConfig = {
			.SPIx = my_SPI1,

			.sckPin = my_GPIO_PIN_5,
			.sckPort = my_GPIOA,

			.nssPin = my_GPIO_PIN_3,
			.nssPort = my_GPIOE,

			.mosiPin = my_GPIO_PIN_7,
			.mosiPort = my_GPIOA,

			.misoPin = my_GPIO_PIN_6,
			.misoPort = my_GPIOA
		};

		SPI_GPIO_init(spiConfig);
		SPI_basicConfigInit(spiConfig, STM32_MASTER, DFF_8BITS, FPCLK_DIV16, SOFTWARE_SLAVE_ENABLE, SPI_ENABLE);
		char spiRead1 = SPI_readReceivedData(spiConfig, 0x0F);
		char spiRead2 = SPI_readReceivedData(spiConfig, 0x20);

		SPI_write2Device(spiConfig, 0x20, 0x0F);
		spiRead2 = SPI_readReceivedData(spiConfig, 0x20);
	}


	else if(strcmp(session, "TIMER") == 0){
		LED_Red_Init();
		LED_Blue_Init();

		while(1){
			LED_Control(LED_Red, 1);
			delay(userDelay);
			LED_Control(LED_Red, 0);
			delay(userDelay);
		}
	}

	else if(strcmp(session, "RCC") == 0){
		RCC_init();
		LED_Blue_Init();
		LED_Red_Init();
		while(1){
			LED_Control(LED_Blue, 1);
			LED_Control(LED_Red, 1);
			delay(userDelay);

			LED_Control(LED_Red, 0);
			LED_Control(LED_Blue, 0);
			delay(userDelay);
		}
	}
}






















