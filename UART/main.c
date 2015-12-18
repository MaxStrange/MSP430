#include <msp430.h> 
#include "my_led.h"
#include "my_uart.h"
#include "main.h"

static int my_delay(void)
{
	volatile unsigned long int i;

	i = 400000;
	do i--;
	while(i != 0);

	return 0;
}

static void setup(void)
{
	init_led();
	init_uart();
}

static void loop(void)
{
	while (1)
	{
		led_blink();
		my_delay();
		uart_write('h');
//		my_delay();
		uart_write('e');
	//	my_delay();
		uart_write('l');
		//my_delay();
		uart_write('l');
		//my_delay();
		uart_write('o');
	}
}

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    setup();
    loop();

	return 0;
}
