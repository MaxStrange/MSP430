#include <msp430.h>
#include "console.h"
#include "my_led.h"
#include "my_strings.h"

static const command allowable_commands[] =
{
		{
				"load\n",
				&led_pattern_loading_bar,
				"Displays a loading bar pattern on the LEDs."
		},

		{
				"blink\n",
				&led_pattern_blink,
				"Displays a blinking pattern on the LEDs."
		},

		{
				"dance\n",
				&led_pattern_dance,
				"Displays a dancing pattern on the LEDs."
		},

		{
				"END_OF_TABLE",
				0x00,
				"End of table indicator"
		}
};


void console_execute_command(const char *cmd)
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
