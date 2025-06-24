#include <stdio.h>
#include <string.h>

#include "leds.h"
#include "buttons.h"
#include "registerAddress.h"
#include "gpio_write_read.h"
#include "interrupt.h"
#include "uart.h"
#include "spi.h"
#include "timer.h"


char session[15] = "EXTI";
int LED_Delay = 400;
uint32_t* desiredOffsetAddr = (uint32_t*)0x20000000;

void EXTIFunction(){
	if(buttonState()){
		LED_Control(LED_Green, 1);
	}
	else{
		LED_Control(LED_Green, 0);
	}
	writeEXTI(0, PR, my_GPIO_PIN_SET); //Clear the flag
}



int main(void){
	HAL_Init();


	if(strcmp(session, "EXTI") == 0){
		buttonInit(0, my_GPIOA);
		LED_Red_Init();
		LED_Green_Init();

		EXTI_TriggerConfig(0, my_EXTI_TRIGGER_BOTH); //Trigger interrupt for rising and falling
		EXTI_Init(0, NVIC_ISER0, EXTI0);
		EXTI_Offset_Init(desiredOffsetAddr);
		user_EXTI_IRQHandler(EXTIFunction, 0x58);

		while(1){
			LED_Control(LED_Red, 1);
			HAL_Delay(LED_Delay);
			LED_Control(LED_Red, 0);
			HAL_Delay(LED_Delay);
		}
	}

	if(strcmp(session, "UART") == 0){
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

	if(strcmp(session, "SPI") == 0){

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

		SPI_GPIO_Init(spiConfig);
		SPI_basicConfigInit(spiConfig, STM32_MASTER, DFF_8BITS, FPCLK_DIV16, SOFTWARE_SLAVE_ENABLE, SPI_ENABLE);
		char spiRead1 = SPI_readReceivedData(spiConfig, 0x0F);
		char spiRead2 = SPI_readReceivedData(spiConfig, 0x20);

		SPI_write2Device(spiConfig, 0x20, 0x0F);
		spiRead2 = SPI_readReceivedData(spiConfig, 0x20);
	}
}
