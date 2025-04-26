/*
 * uart.c
 *
 *  Created on: Apr 23, 2025
 *      Author: dobao
 */
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "stm32f4xx_hal.h"
#include "uart.h"
#include "gpio_write_read.h"
#include "registerAddress.h"

/*
 * UART Initialize in general
 */
void UART_Init(char TXPin,
			   char RXPin,
			   GPIO_PortName_t portName,
			   UART_Name_t UARTx,
			   uint8_t baudRate,
			   UART_Parity_t parity,
			   UART_WordLength_t wordLength){

	/*
	 * Flexible enable UART clock
	 */
	switch(UARTx){
		case my_UART1: __HAL_RCC_USART1_CLK_ENABLE(); break;
		case my_UART2: __HAL_RCC_USART2_CLK_ENABLE(); break;
		case my_UART6: __HAL_RCC_USART6_CLK_ENABLE(); break;
		default: return;
	}

	switch(portName){
		case my_GPIOA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
		case my_GPIOB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
		case my_GPIOC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
		case my_GPIOD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
		case my_GPIOE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
		case my_GPIOH: __HAL_RCC_GPIOH_CLK_ENABLE(); break;
	}
	GPIO_WritePin(TXPin, portName, mode_02, MODER);
	GPIO_WritePin(RXPin, portName, mode_02, MODER);

	if(RXPin <= 7){ //AFRL only contain pin 0 to pin 7
		GPIO_WritePin(TXPin, portName, AF7, AFRL);
		GPIO_WritePin(RXPin, portName, AF7, AFRL);
	}
	else{ //AFRH only contain pin 8 to 15
		GPIO_WritePin(TXPin, portName, AF7, AFRH);
		GPIO_WritePin(RXPin, portName, AF7, AFRH);
	}

	/*
	 * Config baud rate
	 * baud = fclk / (8*(2-OVER8)*UARTDIV)
	 */
	long int f_clk = 16000000L; //16MHz
	char over8 = 0;
	float uartDiv = f_clk / (8*(2-over8) * baudRate);

	int divMantissa = (int)uartDiv; //Extract the integer part
	float divFraction = uartDiv - divMantissa; //Extract the fraction part
	int convert_divFraction = (int)(divFraction * 16);
	uint16_t fullBRR = (divMantissa << 4) | (convert_divFraction & 0xF);

	/*
	 * Write calculated values to BRR
	 */
	GPIO_WriteUART(0, UARTx, BRR, fullBRR);

	/*
	 * Enable TX and RX mode
	 */
	GPIO_WriteUART(2, UARTx, CR1, 1); //Receiver is enabled and begins searching for a start bit
	GPIO_WriteUART(3, UARTx, CR1, 1); //Transmitter enable

	/*
	 * Auto select parity control
	 */
	if(parity == PARITY_NONE){
		GPIO_WriteUART(10, UARTx, CR1, 0); //PCE = 0
	}else{
		GPIO_WriteUART(10, UARTx, CR1, 1); //PCE = 1

		if(parity == PARITY_EVEN){
			GPIO_WriteUART(9, UARTx, CR1, 0); //PS = 0 for EVEN
		} else{
			GPIO_WriteUART(9, UARTx, CR1, 1); //PS = 1 for ODD
		}
	}

	/*
	 * Auto select the data frame size
	 */
	if (wordLength == WORDLENGTH_8B){
		GPIO_WriteUART(12, UARTx, CR1, 0); //Set data frame size as 8 bits
	}else if (wordLength == WORDLENGTH_9B) {
		GPIO_WriteUART(12, UARTx, CR1, 1); //Set data frame size as 9 bits
	}else{
		return;
	}

	GPIO_WriteUART(13, UARTx, CR1, 1); //Enable UART
}







