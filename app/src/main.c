/*
 * main.c
 *
 *  Created on: Jul 23, 2023
 *      Author: LECLERCQ Antonin
 */


#include "main.h"

static void SystemClock_Config(void);

int main(void) {

	SystemClock_Config();

	TIM6_Delay_Init();

	USART_init();

	m_printf("CS5509 16bit ADC Test\r\n");

	TIM6_Delay_SetPrescaler(MILLI_PRE);
	TIM6_Delay(100); // Delay 100ms for the ADC to settle

	CS5509_Init();
	CS5509_ClockEnable();
	CS5509_Calibrate(); // Must calibrate ADC before converting

	uint16_t adc_val = 0;

	while(1) {
		if (CS5509_Data_Ready()) {
			adc_val = CS5509_Data_Receive();
			m_printf("ADC Value = %d	\r", adc_val);
		}
	}
	return 0;
}


static void SystemClock_Config(void) {
	/*
	 * 16MHz system clock
	 */

	uint32_t	HSE_Status;
	uint32_t	PLL_Status;
	uint32_t	SW_Status;
	uint32_t	timeout = 0;

	timeout = 1000000;

	// Start HSE in Bypass Mode
	RCC->CR |= RCC_CR_HSEBYP;
	RCC->CR |= RCC_CR_HSEON;

	// Wait until HSE is ready
	do
	{
		HSE_Status = RCC->CR & RCC_CR_HSERDY_Msk;
		timeout--;
	} while ((HSE_Status == 0) && (timeout > 0));

	// Select HSE as PLL input source
	RCC->CFGR &= ~RCC_CFGR_PLLSRC_Msk;
	RCC->CFGR |= (0x02 <<RCC_CFGR_PLLSRC_Pos);

	// Set PLL PREDIV to /1
	RCC->CFGR2 = 0x00000000;

	// Set PLL MUL to x2
	RCC->CFGR &= ~RCC_CFGR_PLLMUL_Msk;
	RCC->CFGR |= (0x00 <<RCC_CFGR_PLLMUL_Pos);

	// Enable the main PLL
	RCC-> CR |= RCC_CR_PLLON;

	// Wait until PLL is ready
	do
	{
		PLL_Status = RCC->CR & RCC_CR_PLLRDY_Msk;
		timeout--;
	} while ((PLL_Status == 0) && (timeout > 0));

		// Set AHB prescaler to /1
	RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	//Set APB1 prescaler to /1
	RCC->CFGR &= ~RCC_CFGR_PPRE_Msk;
	RCC->CFGR |= RCC_CFGR_PPRE_DIV1;

	// Enable FLASH Prefetch Buffer
	FLASH->ACR |= FLASH_ACR_PRFTBE;

	// Select the main PLL as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	// Wait until PLL becomes main switch input
	do
	{
		SW_Status = (RCC->CFGR & RCC_CFGR_SWS_Msk);
		timeout--;
	} while ((SW_Status != RCC_CFGR_SWS_PLL) && (timeout > 0));

	// Update SystemCoreClock global variable
	SystemCoreClockUpdate();
}
