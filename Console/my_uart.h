/*
 * my_uart.h
 *
 *  Created on: Dec 16, 2015
 *      Author: Max
 */

#ifndef MY_UART_H_
#define MY_UART_H_

void uart_init(void);
void uart_write(char *str);



static void init_pins(void);
static void init_state(void);
static void load_received_byte_into_tx(void);//used for echoing
static void send_next_byte(void);



#endif /* MY_UART_H_ */
