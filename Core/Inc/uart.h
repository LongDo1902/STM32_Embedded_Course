/*
 * UART.h
 *
 *  Created on: Apr 23, 2025
 *      Author: dobao
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "gpio_write_read.h"

/*
 * Function Declarations
 */
void UART_Init(char TXPin,
			   char RXPin,
			   GPIO_PortName_t portName,
			   UART_Name_t UARTx,
			   uint8_t baudRate,
			   UART_Parity_t parity,
			   UART_WordLength_t wordLength);

#endif /* INC_UART_H_ */
