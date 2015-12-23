#include <msp430.h>
#include "my_clock.h"
#include "my_uart.h"
#include "my_led.h"
#include "my_system.h"
#include "my_strings.h"

//TODO : get my dad to show me how to do this better with function pointers


#define COMMAND_LENGTH 20
static char command[COMMAND_LENGTH];

//TODO : should really chomp each word as it comes in to strip the \n

#define NUMBER_OF_COMMANDS 4
static char *allowable_commands[NUMBER_OF_COMMANDS] =
{
		"load\n",
		"blink\n",
		"dance\n",
		"help\n"
};

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    clock_init();
    led_init();
    uart_init();
    _enable_interrupts();

    while (1)
    {
    	/*
    	 * Reinitialize command string.
    	 */
    	unsigned int i = 0;
    	for (i = 0; i < COMMAND_LENGTH; i++)
    	{
    		command[i] = 0;
    	}


    	uart_get_console_input(command, COMMAND_LENGTH);
    	uart_write("You just typed: ");
    	uart_write(command);
    	system_delay(500);//give time for the uart to output

    	if (strings_compare(command, "load\n"))
    		led_pattern_loading_bar();
    	else if (strings_compare(command, "blink\n"))
    		led_pattern_blink();
    	else if (strings_compare(command, "dance\n"))
    		led_pattern_dance();
    	else
    		uart_write("Usage: load, blink, or dance");
    }
}
