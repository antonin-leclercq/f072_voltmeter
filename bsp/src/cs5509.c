/*
 * cs5509.c
 *
 *  Created on: Jul 23, 2023
 *      Author: LECLERCQ Antonin
 */


#include "cs5509.h"

void CS5509_Init(void) {
	/*
	 * APB2 peripherals running at 16MHz
	 * SPI1 baud rate at 250kHz (/64 prescaler) --> maximum is 1.25MHz at 3.3V for CS5509
	 *
	 *  PA5 (SPI1_SCK)
	 *  PA6 (SPI1_MISO)
	 *  PA7 (SPI1_MOSI)
	 *
	 *  PC5 --> software controlled chip select
	 *  PC6 --> CS5509 CONV input pin
	 *  PC7 --> CS5509 DRDY output pin
	 *  PC8 --> CS5509 CAL input pin
	 *
	 *  PA8 --> CS5509 CLK input pin
	 */

	// Set PA5 , PA6 and PA7 as AF0
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= ~(GPIO_MODER_MODER5_Msk | GPIO_MODER_MODER6_Msk | GPIO_MODER_MODER7_Msk);
	GPIOA->MODER |= (0x02 << GPIO_MODER_MODER5_Pos) | (0x02 << GPIO_MODER_MODER6_Pos) | (0x02 << GPIO_MODER_MODER7_Pos);
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL5_Msk | GPIO_AFRL_AFRL6_Msk | GPIO_AFRL_AFRL7_Msk);

	// Set PC5 (medium speed), PC6 and PC8 as output, PC7 as input
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER &= ~(GPIO_MODER_MODER5_Msk | GPIO_MODER_MODER6_Msk | GPIO_MODER_MODER7_Msk | GPIO_MODER_MODER8_Msk);
	GPIOC->MODER |= (0x01 << GPIO_MODER_MODER5_Pos) | (0x01 << GPIO_MODER_MODER6_Pos) | (0x01 << GPIO_MODER_MODER8_Pos);
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEEDR5_Msk;
	GPIOC->OSPEEDR |= (0x01 << GPIO_OSPEEDR_OSPEEDR5_Pos);

	// Set CS High
	GPIOC->ODR |= GPIO_ODR_5;

	// Enable SPI1
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	// Reset Config
	SPI1->CR1 = 0x0000;
	SPI1->CR2 = 0x0700;

	// Enable software slave management, MCU is master
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR;

	// Set prescaler to /16 for 250kHz operation
	SPI1->CR1 |= (0x05 << SPI_CR1_BR_Pos);

	// Select 16 bit data length
	SPI1->CR2 &= ~SPI_CR2_DS_Msk;
	SPI1->CR2 |= (0x0F << SPI_CR2_DS_Pos);

	// Enable SPI1
	SPI1->CR1 |= SPI_CR1_SPE;
}

void CS5509_Calibrate(void) {
	/*
	 * 1) CAL High
	 * 2) CONV High
	 * 3) Wait a bit
	 * 4) CAL Low
	 * 5) Wait 100ms
	 *
	 * CONV is left high afterwards for continuous operation
	 */

	TIM6_Delay_SetPrescaler(MICRO_PRE);

	GPIOC->ODR |= GPIO_ODR_8;
	GPIOC->ODR |= GPIO_ODR_6;

	TIM6_Delay(250); // Arbitrary value, in µs
	TIM6_Delay_SetPrescaler(MILLI_PRE); // Change to ms

	GPIOC->ODR &= ~GPIO_ODR_8;

	// Calibrating
	TIM6_Delay(100);
}

void CS5509_ClockEnable(void) {
	/*
	 * Using external LSE oscillator running at 32768Hz
	 */

	// Enable the PWR clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;

	// Enable access to RTC and Backup registers
	PWR->CR |= PWR_CR_DBP;

	// Resets Backup Domain Config
	RCC->BDCR |= RCC_BDCR_BDRST;
	RCC->BDCR &= ~RCC_BDCR_BDRST;

	// Select LSE medium high drive capability
	RCC->BDCR &= ~RCC_BDCR_LSEDRV_Msk;
	RCC->BDCR |= (0x01 << RCC_BDCR_LSEDRV_Pos);

	// Enable LSE
	RCC->BDCR |= RCC_BDCR_LSEON;
	while((RCC->BDCR & RCC_BDCR_LSERDY) != RCC_BDCR_LSERDY);

	// Set PA8 as MCO output (AF0)
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODER8_Msk;
	GPIOA->MODER |= (0x02 << GPIO_MODER_MODER8_Pos);
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFRH0_Msk;

	// Output LSE on PA8
	RCC->CFGR &= ~(RCC_CFGR_MCO_Msk | RCC_CFGR_MCOPRE_Msk);
	RCC->CFGR |= (0x03 << RCC_CFGR_MCO_Pos);
}

uint8_t CS5509_Data_Ready(void) {
	return (uint8_t)((GPIOC->IDR & GPIO_IDR_7) == 0);
}

uint16_t CS5509_Data_Receive(void) {
	static uint16_t	rx_byte = 0;

	// Set CS low
	GPIOC->ODR &= ~GPIO_ODR_5;

	// Make sure TXE is set before sending data
	while((SPI1->SR & SPI_SR_TXE_Msk) == 0);

	// Send tx_byte (0x00)
	SPI1->DR = 0x0000;

	// Wait until incoming data has arrived
	while((SPI1->SR & SPI_SR_RXNE_Msk) == 0);

	// Read data
	rx_byte = SPI1->DR;

	// Set CS high again
	GPIOC->ODR |= GPIO_ODR_5;

	return rx_byte;
}
