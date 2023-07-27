/*
 * main.h
 *
 *  Created on: Jul 23, 2023
 *      Author: LECLERCQ Antonin
 */

#ifndef APP_INC_MAIN_H_
#define APP_INC_MAIN_H_

#include "stm32f0xx.h"
#include "cs5509.h"
#include "delay.h"
#include "usart.h"

extern int m_printf(const char *format, ...);
extern int m_sprintf(char *out, const char *format, ...);

#endif /* APP_INC_MAIN_H_ */
