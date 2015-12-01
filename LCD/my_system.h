/*
 * my_system.c
 *
 *  Created on: Nov 29, 2015
 *      Author: Max
 */

#ifndef MY_SYSTEM_H_
#define MY_SYSTEM_H_

#include <msp430.h>

void delay_ms(unsigned int ms);
char* itoa(int value, char *result, int base);

#endif /*MY_SYSTEM_H_*/
