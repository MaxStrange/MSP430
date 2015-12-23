#include <msp430.h>
#include "console.h"
#include "my_led.h"
#include "my_strings.h"
#include "my_uart.h"
#include "my_system.h"

static const char *help_str = "Usage: load, blink, dance. \n";
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

const char * console_get_help_str(void)
{
	return help_str;
}

/*
 * Attempts get a command from the user and execute it.
 * Returns 1 (true) on successful execution of a function
 * from the command table and 0 (false) otherwise.
 */
int console_go(void)//bool
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
	uart_write("\n");
	system_delay(500);//give time for the uart to output

	int worked = execute_command(console_input);

	return worked;
}

/*
 * Executes the given command's function. If successful,
 * returns 1 (true), otherwise, returns 0 (false).
 */
static int execute_command(const char *cmd)//bool
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
			return 1;//true - found the function and executed it
		}
		else
		{
			i++;
		}
	}

	return 0;//false - never found the function
}
