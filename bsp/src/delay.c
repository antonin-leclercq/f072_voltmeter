/*
 * delay.c
 *
 *  Created on: Jul 23, 2023
 *      Author: LECLERCQ Antonin
 */

#include "delay.h"

void TIM6_Delay_Init(void) {
	// Enable TIM6 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	// Reset TIM6 configuration
	TIM6->CR1 = 0x0000;
	TIM6->CR2 = 0x0000;

	// Set default TIM6 prescaler
	TIM6->PSC = (uint16_t) DEFAULT_PRE -1;

	// Set ARR to maximum value
	TIM6->ARR = (uint16_t) 0xFFFF;
}

void TIM6_Delay_SetPrescaler(uint16_t p) {
	// Stop TIM6 counter
	TIM6->CR1 &= ~TIM_CR1_CEN;
	TIM6->PSC = (uint16_t) p -1;
}

void TIM6_Delay(uint32_t d) {
	// Resets TIM6 counter
	TIM6->EGR |= TIM_EGR_UG;

	// Start TIM6 counter
	TIM6->CR1 |= TIM_CR1_CEN;

	// Wait until TIM6 counter reaches delay
	while(TIM6->CNT < d);

	// Stop TIM6 counter
	TIM6->CR1 &= ~TIM_CR1_CEN;
}
