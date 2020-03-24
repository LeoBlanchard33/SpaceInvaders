#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "serial.h"

static volatile unsigned char last_char_read = 1;
static volatile char last_char;

void serial_init(const uint32_t baudrate)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Use USART2 with PA2 and PA3 */

	/* Enable clock for GPIOA and USART */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Init GPIOA 2 and 3 as alternate functions */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Connect TX and RX pints to their alternate function pins */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	/* Configure USART parameters */
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);

	/* Configure interruption */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2, ENABLE);
}

void serial_putchar(const volatile char c)
{
	/* Check USART */
	if ((USART2->CR1 & USART_CR1_UE))
	{
		/* Wait to be ready, buffer empty */
		while (!(USART2->SR & USART_FLAG_TXE))
			;
		/* Send data */
		USART2->DR = (uint16_t) (c & 0x01FF);
		/* Wait to be ready, buffer empty */
		while (!(USART2->SR & USART_FLAG_TXE))
			;
	}
}

void serial_puts(const volatile char *s)
{
	while (*s)
		serial_putchar(*s++);
}

signed char serial_get_last_char(void)
    {
	if (last_char_read)
		return -1;
	else
	{
		last_char_read = 1;
		return last_char;
	}
}

/* interruption management */

static int32_t serial_input_not_empty(void)
{
	return USART_GetITStatus(USART2, USART_IT_RXNE);
}

static char serial_input_character(void)
{
	return USART2->DR;
}

#define serial_it_handler USART2_IRQHandler
void serial_it_handler(void)
{
	if (serial_input_not_empty())
	{
		last_char = serial_input_character();
		last_char_read = 0;
	}
}

