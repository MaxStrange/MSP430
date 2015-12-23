#include <msp430.h>
#include "my_clock.h"
#include "my_uart.h"
#include "my_led.h"
#include "my_system.h"
#include "my_strings.h"

//TODO : get my dad to show me how to do this better with function pointers

#define command_length 20
static char the_command[] = "blah\n";
static char command[command_length];

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    clock_init();
    led_init();
    uart_init();
    _enable_interrupts();

    while (1)
    {
    	unsigned int i = 0;
    	for (i = 0; i < command_length; i++)
    	{
    		command[i] = 0;
    	}


    	uart_get_console_input(command, command_length);
    	uart_write("You just typed: ");
    	uart_write(command);
    	system_delay(500);

    	int tf = strings_compare(command, the_command);

    	if (tf)
    	{
        	led_toggle_led(LED1);
        	system_delay(100);
        	led_toggle_led(LED2);
        	system_delay(100);
        	led_toggle_led(LED3);
        	system_delay(100);
        	led_toggle_led(LED4);
        	system_delay(100);
        	led_toggle_led(LED5);
        	system_delay(100);
        	led_toggle_led(LED6);
        	system_delay(100);
        	led_toggle_led(LED7);
        	system_delay(100);
        	led_toggle_led(LED8);
        	system_delay(100);
    	}
    	else
    	{
    		led_toggle_all();
    		system_delay(250);
    		led_toggle_all();
    	}
    }
}
