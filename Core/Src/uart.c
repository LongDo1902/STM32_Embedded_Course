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
			   uint16_t baudRate,
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

	/*
	 * Flexible enable GPIO port clock
	 */
	switch(portName){
		case my_GPIOA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
		case my_GPIOB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
		case my_GPIOC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
		case my_GPIOD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
		case my_GPIOE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
		case my_GPIOH: __HAL_RCC_GPIOH_CLK_ENABLE(); break;
	}

	/*
	 * Set TX/RXpin of a specific port to MODER mode and set its state to 0x02 (Alternate function mode)
	 */
	GPIO_WritePin(TXPin, portName, MODER, mode_02);
	GPIO_WritePin(RXPin, portName, MODER, mode_02);


	if(RXPin <= 7){ //AFRL only contain pin 0 to pin 7
		GPIO_WritePin(TXPin, portName, AFRL, AF7);
		GPIO_WritePin(RXPin, portName, AFRL, AF7);
	}

	else{ //AFRH only contain pin 8 to 15
		GPIO_WritePin(TXPin, portName, AFRH, AF7);
		GPIO_WritePin(RXPin, portName, AFRH, AF7);
	}

	/*
	 * CONFIG UART
	 *
	 * Config baud rate
	 * baud = fclk / (8*(2-OVER8)*UARTDIV)
	 */
	long int f_clk = 16000000L; //16MHz
	char over8 = 0; //16x oversampling
	float uartDiv = f_clk / (8*(2-over8) * baudRate);

	int divMantissa = (int)uartDiv; //Extract the integer part
	float divFraction = uartDiv - divMantissa; //Extract the fraction part
	int convert_divFraction = (int)(divFraction * 16); //scale and store divFraction in 4 bits
	uint16_t fullBRR = (divMantissa << 4) | (convert_divFraction & 0xF);

	/*
	 * Write calculated full values to BRR
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



/*
 * Receives a character over the specified UART interface
 *
 * This function waits until data is available in the receive buffer (RXNE == 1)
 * Checks for a parity error (PE == 1), and if there is no error, returns the received byte
 * If a parity error occurs, it discards the corrupted data and return -1.
 */
char my_UART_Receive(UART_Name_t UARTx){
	//Wait until the data is ready
	while(readUART(5, UARTx, SR) == 0); //Wait for RXNE == 1

	/*
	 * To clear the PE, FE, NE, or ORE flags, the following sequence must be respected:
	 * Read the SR register
	 * Read the DR register
	 *
	 * If skip DR read:
	 * 		The flag won't be cleared
	 * 		You may get stuck in your parity check loop
	 * 		Worst case: DR wont get new data, leading to data loss or lockup
	 */

	//Check parity error (bitPosition = 0)
	if(readUART(0, UARTx, SR) == 1){
		(void)readUART(0, UARTx, DR); //read and discard error
		return -1;
	}

	//No error, return valid data
	char data = readUART(0, UARTx, DR);
	return data;
}

/*
 * inputData has 8 bits long
 */
void my_UART_Transmit(UART_Name_t UARTx, uint8_t inputData){
	while((readUART(7, UARTx, SR) & 1) == 0); //Check if transmit data register is empty, == 0 means not empty
	GPIO_WriteUART(0, UARTx, DR, inputData); //Write inputData to DR register.
	while((readUART(6, UARTx, SR) & 1) == 0); //Check if transmission is completed.
}


