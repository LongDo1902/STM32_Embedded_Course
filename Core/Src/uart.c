/*
 * uart.c
 *
 *  Created on: Apr 23, 2025
 *      Author: dobao
 */
#include "uart.h"


/*
 * UART Initialize in general
 */
void UART_Init(GPIO_Pin_t TXPin,
			   GPIO_Pin_t RXPin,
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
		default: return; //Other cases
	}

	/*
	 * Set TX/RXpin of a specific port to MODER mode and set its state to 0x02 (Alternate function mode)
	 */
	GPIO_WritePin(TXPin, portName, MODER, mode_02);
	GPIO_WritePin(RXPin, portName, MODER, mode_02);

	GPIO_Mode_t afrRegTX = (TXPin <= 7) ? AFRL : AFRH;
	GPIO_Mode_t afrRegRX = (RXPin <= 7) ? AFRL : AFRH;

	GPIO_WritePin(TXPin, portName, afrRegTX, AF7);
	GPIO_WritePin(RXPin, portName, afrRegRX, AF7);

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
	WriteUART(0, UARTx, BRR, fullBRR);

	/*
	 * Enable TX and RX mode
	 */
	WriteUART(2, UARTx, CR1, 1); //Receiver is enabled and begins searching for a start bit
	WriteUART(3, UARTx, CR1, 1); //Transmitter enable

	/*
	 * Auto select parity control
	 */
	if(parity == PARITY_NONE){
		WriteUART(10, UARTx, CR1, 0); //PCE = 0
	}else{
		WriteUART(10, UARTx, CR1, 1); //PCE = 1

		if(parity == PARITY_EVEN){
			WriteUART(9, UARTx, CR1, 0); //PS = 0 for EVEN
		} else{
			WriteUART(9, UARTx, CR1, 1); //PS = 1 for ODD
		}
	}

	/*
	 * Auto select the data frame size
	 */
	if (wordLength == WORDLENGTH_8B){
		WriteUART(12, UARTx, CR1, 0); //Set data frame size as 8 bits
	}else if (wordLength == WORDLENGTH_9B) {
		WriteUART(12, UARTx, CR1, 1); //Set data frame size as 9 bits
	}else{
		return;
	}

	WriteUART(13, UARTx, CR1, 1); //Enable UART
}



/*
 * @brief	Receives a character over the specified UART interface
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
 * 	@brief	Send one byte over a selected UART port
 * 	@param 	Target peripheral: my_UART1, my_UART2, my_UART6.
 * 	@param	inputData: write any value/data to but only <= 1 byte
 *
 * 	Routine:
 * 		1. Waits for TXE = 1 (DR empty)
 * 		2. Writes the inputData to DR register
 * 		3. Wait for TC = 1 to ensure the byte has completely left the shift register.
 */
void my_UART_Transmit(UART_Name_t UARTx, uint8_t inputData){
	while((readUART(7, UARTx, SR) & 1) == 0);
	WriteUART(0, UARTx, DR, inputData & 0x1FF);
	while((readUART(6, UARTx, SR) & 1) == 0);
}



/*
 * Helper function to write bit to pins to config UART
 * @param	bitPosition		bit location that you want to write
 * @param	userUARTx		write my_UART1 if want to write UART1
 * @param	mode			different UART mode registers
 * @param	state			write set for 1, reset for 0
 */
void WriteUART(uint8_t bitPosition, UART_Name_t userUARTx, UART_Mode_t mode, uint32_t value){
	UART_Register_Offset_t* UARTx;
	switch(userUARTx){
		case my_UART1: UARTx = UART1_REG; break;
		case my_UART2: UARTx = UART2_REG; break;
		case my_UART6: UARTx = UART6_REG; break;
		default: return;
	}

	volatile uint32_t* reg;
	switch(mode){
		case DR: reg = &UARTx -> DR; break;
		case BRR: reg = &UARTx -> BRR; break;
		case CR1: reg = &UARTx -> CR1; break;
		case CR2: reg = &UARTx -> CR2; break;
		case CR3: reg = &UARTx -> CR3; break;
		case GTPR: reg = &UARTx -> GTPR; break;
		default: return;
	}

	//Auto detect bitwidth based on the "value" length
	uint8_t bitWidth = 0;
	uint32_t temp = value;
	while(temp > 0){
		bitWidth++; //Increment the bidWidth by one when ever temp > 0
		temp = temp >> 1; //Shift the temp to the right by 1
		//temp >>= 1;
	}

	if(value == 0 && bitWidth == 0) bitWidth = 1; //Allowing clearing bits

	//Prevent overflow
	if (bitPosition + bitWidth > 32) return;

	//Mask off the old bit and OR with new value
	uint32_t mask = ((1U << bitWidth) - 1U) << bitPosition;
	uint32_t shiftedValue = (value << bitPosition) & mask;
	*reg = (*reg & ~mask) | shiftedValue;
}



/*
 * Reads the status of a specific bit from a UART peripheral register.
 *
 *  @param	bitPosition	Bit position to read (0-31).
 *  @param	userUARTx	UART peripheral to access (e.g., my_UART1, my_UART2, my_UART6).
 *  @param	mode		UART register to read from (e.g., SR, DR, CR1, etc.).
 *
 * Returns:
 *  	1 if the specified bit is set (bit = 1).
 *  	0 if the specified bit is cleared (bit = 0).
 *  	-1 if an invalid UART peripheral or register mode is selected.
 */
char readUART(uint8_t bitPosition, UART_Name_t userUARTx, UART_Mode_t mode){
	UART_Register_Offset_t* UARTx;
	switch(userUARTx){
		case my_UART1: UARTx = UART1_REG; break;
		case my_UART2: UARTx = UART2_REG; break;
		case my_UART6: UARTx = UART6_REG; break;
		default: return -1; //return an error value
	}

	volatile uint32_t* reg;
	switch(mode){
		case SR: reg = &UARTx -> SR; break; //R
		case DR: reg = &UARTx -> DR; break; //RW
		case BRR: reg = &UARTx -> BRR; break; //RW
		case CR1: reg = &UARTx -> CR1; break; //RW
		case CR2: reg = &UARTx -> CR2; break; //RW
		case CR3: reg = &UARTx -> CR3; break; //RW
		case GTPR: reg = &UARTx -> GTPR; break; //RW
		default: return -1;
	}

	//Different mode has different behavior
	if (mode == DR){
		return (char)(*reg & 0xFF);
	}

	else{
		if(((*reg) >> bitPosition) & 0x1){
			return 1;
		} else{
			return 0;
		}
	}
}
