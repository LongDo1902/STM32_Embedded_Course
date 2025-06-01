/*
 * gpio_write_read.h
 *
 *  Created on: Apr 13, 2025
 *      Author: dobao
 */

#ifndef INC_GPIO_WRITE_READ_H_
#define INC_GPIO_WRITE_READ_H_
#include <stdint.h>

/*
 * Collections of pins 0 to 15
 */
typedef enum{
	my_GPIO_PIN_0 = 0,
	my_GPIO_PIN_1,
	my_GPIO_PIN_2,
	my_GPIO_PIN_3,
	my_GPIO_PIN_4,
	my_GPIO_PIN_5,
	my_GPIO_PIN_6,
	my_GPIO_PIN_7,
	my_GPIO_PIN_8,
	my_GPIO_PIN_9,
	my_GPIO_PIN_10,
	my_GPIO_PIN_11,
	my_GPIO_PIN_12,
	my_GPIO_PIN_13,
	my_GPIO_PIN_14,
	my_GPIO_PIN_15
}GPIO_Pin_t;

/*
 * Collections of ports from A to H
 */
typedef enum{
	my_GPIOA,
	my_GPIOB,
	my_GPIOC,
	my_GPIOD,
	my_GPIOE,
	my_GPIOH
}GPIO_PortName_t;

/*
 * Collections of different pin's states
 */
typedef enum{
	my_GPIO_PIN_SET = 1, 	//High
	my_GPIO_PIN_RESET = 0,	//Low

	mode_00 = 0x00, //0b00
	mode_01 = 0x01, //0x01
	mode_02 = 0x02, //0b10
	mode_03 = 0x03, //0b11

	//Alternate Function:
	AF0 = 0x0000,
	AF1,
	AF2,
	AF3,
	AF4,
	AF5,	//0x0101
	AF6,
	AF7,
	AF8,
	AF9,
	AF10,	//0x1010
	AF11,
	AF12,
	AF13,
	AF14,
	AF15	//0x1111
}GPIO_State_t;

/*
 * Collections of GPIO Offset Register Name
 */
typedef enum{MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, AFRL, AFRH}GPIO_Mode_t;

/*
 * Collections of EXTI Offset Register Name
 */
typedef enum{IMR, EMR, RTSR, FTSR, SWIER, PR}EXTI_Mode_t;

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
 * Collection of Flash Interface Register Name
 */
typedef enum{FLASH_ACR, FLASH_KEYR, FLASH_OPTKEYR, FLASH_SR, FLASH_CR, FLASH_OPTCR} Flash_IntF_Mode_t;
////////////////END OF DIFFERENT MODES REG////////////////


/*
 * Function declarations
 */
void GPIO_WritePin (GPIO_Pin_t pinNum,
					GPIO_PortName_t port,
					GPIO_Mode_t mode,
					GPIO_State_t state);

void GPIO_WriteEXTI(uint8_t bitPosition,
					EXTI_Mode_t mode,
					GPIO_State_t state);

void GPIO_WriteUART(uint8_t bitPosition,
					UART_Name_t userUARTx,
					UART_Mode_t mode,
					uint32_t value);

char readUART(uint8_t bitPosition,
			  UART_Name_t userUARTx,
			  UART_Mode_t mode);

char readPin(uint8_t bitPosition,
			GPIO_PortName_t port,
			GPIO_Mode_t mode);

void GPIO_WriteFlash(uint8_t bitPosition,
					Flash_IntF_Mode_t mode,
					uint32_t value);

char readFLASH(uint8_t bitPosition, Flash_IntF_Mode_t mode);

#endif /* INC_GPIO_WRITE_READ_H_ */
