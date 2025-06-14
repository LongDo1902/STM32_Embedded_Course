/*
 * UART.h
 *
 *  Created on: Apr 23, 2025
 *      Author: dobao
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "stm32f4xx_hal.h"
#include "gpio_write_read.h"
#include "registerAddress.h"


/*
 * Collections of UART Offset Register Name
 */
typedef enum{SR, DR, BRR, CR1, CR2, CR3, GTPR}UART_Mode_t;

/*
 * Collections of UARTx name
 */
typedef enum{
	my_UART1,
	my_UART2,
	my_UART6
}UART_Name_t;

/*
 * Collections of parity modes
 */
typedef enum{
	PARITY_NONE,
	PARITY_EVEN,
	PARITY_ODD
}UART_Parity_t;

/*
 * Collection of data frames (data size)
 */
typedef enum{
	WORDLENGTH_8B,
	WORDLENGTH_9B
}UART_WordLength_t;



/*
 * Function Declarations
 */
void WriteUART(uint8_t bitPosition, UART_Name_t userUARTx, UART_Mode_t mode, uint32_t value);

char readUART(uint8_t bitPosition, UART_Name_t userUARTx, UART_Mode_t mode);

void UART_Init(GPIO_Pin_t TXPin,
			   GPIO_Pin_t RXPin,
			   GPIO_PortName_t portName,
			   UART_Name_t UARTx,
			   uint16_t baudRate,
			   UART_Parity_t parity,
			   UART_WordLength_t wordLength);

char my_UART_Receive(UART_Name_t UARTx);

void my_UART_Transmit(UART_Name_t UARTx, uint8_t inputData);

#endif /* INC_UART_H_ */
