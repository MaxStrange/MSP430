#include <msp430.h>
#include "my_clock.h"
#include "my_uart.h"

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    clock_init();
    uart_init();

    //_enable_interrupts();

    while (1)
    {
    	uart_write("hello!");

    	volatile unsigned int i = 50000;
    	for (; i > 0; i--)
    		;
    }
}
