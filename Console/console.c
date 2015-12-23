#include <msp430.h>
#include "console.h"
#include "my_led.h"
#include "my_strings.h"
#include "my_uart.h"
#include "my_system.h"


static char console_input[INPUT_LENGTH];

static const command allowable_commands[] =
{
		{
				"load",
				&led_pattern_loading_bar,
				"Displays a loading bar pattern on the LEDs."
		},

		{
				"blink",
				&led_pattern_blink,
				"Displays a blinking pattern on the LEDs."
		},

		{
				"dance",
				&led_pattern_dance,
				"Displays a dancing pattern on the LEDs."
		},

		{
				"END_OF_TABLE",
				0x00,
				"End of table indicator"
		}
};


void console_go(void)
{
	/*
	 * Reinitialize console string.
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

	execute_command(console_input);
}

static void execute_command(const char *cmd)
{
	unsigned int i = 0;
	const char *name = allowable_commands[i].name;
	while (!strings_compare(name, "END_OF_TABLE"))
	{
		name = allowable_commands[i].name;
		if (strings_compare(cmd, name))
		{
			function_pointer p = allowable_commands[i].execute;
			(*p)();
			return;
		}
		else
		{
			i++;
		}
	}
}
