/*
 * my_uart.h
 *
 *  Created on: Dec 16, 2015
 *      Author: Max
 */

#ifndef MY_UART_H_
#define MY_UART_H_

#define NUM_ELEMENTS_IN_TX_BUFFER 100

void uart_init(void);
void uart_write(char *str);



static void init_pins(void);
static void send_next_byte(void);



#endif /* MY_UART_H_ */
