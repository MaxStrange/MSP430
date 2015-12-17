#include <msp430.h>
#include "my_clock.h"
#include "my_uart.h"

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    clock_init();
    uart_init();

    while (1)
    {
    	uart_write("hello!");
    }
}
