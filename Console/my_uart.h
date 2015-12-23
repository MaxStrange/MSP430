/*
 * my_uart.h
 *
 *  Created on: Dec 16, 2015
 *      Author: Max
 */

#ifndef MY_UART_H_
#define MY_UART_H_

/*
 * API functions
 */

void uart_get_console_input(char *buffer, unsigned int buffer_length);
void uart_init(void);
void uart_write(const char *str);


/*
 * Static helper functions
 */

static void get_user_input(void);
static void init_pins(void);
static void init_state(void);
static void read_from_rx_into_str(char *str, unsigned int str_length);
static void reinitialize_rx_queue(void);


/*
 * ISR helpers
 */

static inline void read_data_into_rx_buffer(void);
static inline void send_next_byte(void);



#endif /* MY_UART_H_ */
