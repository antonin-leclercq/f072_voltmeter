/*
 * cs5509.h
 *
 *  Created on: Jul 23, 2023
 *      Author: LECLERCQ Antonin
 */

#ifndef BSP_INC_CS5509_H_
#define BSP_INC_CS5509_H_

#include "stm32f0xx.h"
#include "delay.h"

void CS5509_Init(void);

void CS5509_ClockEnable(void);

void CS5509_Calibrate(void);

uint8_t CS5509_Data_Ready(void);
uint16_t CS5509_Data_Receive(void);

#endif /* BSP_INC_CS5509_H_ */
