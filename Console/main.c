#include <msp430.h>
#include "my_clock.h"
#include "my_uart.h"
#include "my_led.h"
#include "my_system.h"
#include "my_strings.h"
#include "console.h"

#define INPUT_LENGTH 20
static char console_input[INPUT_LENGTH];

//TODO : should really chomp each word as it comes in to strip the \n


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    clock_init();
    led_init();
    uart_init();
    _enable_interrupts();

    while (1)
    {
    	/*
    	 * Reinitialize console input string.
    	 */
    	unsigned int i = 0;
    	for (i = 0; i < INPUT_LENGTH; i++)
    	{
    		console_input[i] = 0;
    	}


    	uart_get_console_input(console_input, INPUT_LENGTH);
    	uart_write("You just typed: ");
    	uart_write(console_input);
    	system_delay(500);//give time for the uart to output

    	console_execute_command(console_input);
    }
}
