/*
 * delay.h
 *
 *  Created on: Jul 23, 2023
 *      Author: LECLERCQ Antonin
 */

#ifndef BSP_INC_DELAY_H_
#define BSP_INC_DELAY_H_

#include "stm32f0xx.h"

#define MILLI_PRE 16000
#define MICRO_PRE 16
#define DEFAULT_PRE MILLI_PRE

void TIM6_Delay_Init(void);

void TIM6_Delay_SetPrescaler(uint16_t p);

void TIM6_Delay(uint32_t d);

#endif /* BSP_INC_DELAY_H_ */
