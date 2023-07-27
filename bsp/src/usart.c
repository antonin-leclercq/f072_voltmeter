/*
 * usart.c
 *
 *  Created on: Jul 23, 2023
 *      Author: LECLERCQ Antonin
 */


#include "usart.h"

void USART_init(void)
{
	/*
	 * USART2 on PA2 (TX), PA3 (RX), AF1
	 * 9600 bauds
	 * USARTDIV = Fck / baud = 16MHz / 9600 = 1667 (0.2% error)
	 */

	// Enable GPIOA clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// Set PA2 and PA3 to AF1
	GPIOA->MODER &= ~(GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk);
	GPIOA->MODER |= (0x02 << GPIO_MODER_MODER2_Pos) | (0x02 << GPIO_MODER_MODER3_Pos);

	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2_Msk | GPIO_AFRL_AFRL3_Msk);
	GPIOA->AFR[0] |= (0x01 << GPIO_AFRL_AFRL2_Pos) | (0x01 << GPIO_AFRL_AFRL3_Pos);

	// Enable USART2 clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	// Reset USART2 configuration
	USART2->CR1 = 0x00000000;
	USART2->CR2 = 0x00000000;
	USART2->CR3 = 0x00000000;

	// Set BRR
	USART2->BRR = (uint16_t)USART_DIV;

	// Enable Tx and Rx
	USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;

	// Enable USART2
	USART2->CR1 |= USART_CR1_UE;
}
