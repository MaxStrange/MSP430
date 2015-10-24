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
//	unsigned char tx_buffer[6] = "hello"; -- look up strcpy for assigning strings to char arrays
	unsigned char i = 0;


	while (1)
	{
		led_blink();
		my_delay();
//		uart_write(tx_buffer[i]);
		uart_write('h');

		i++;
		if (i >= 6) i = 0;
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
