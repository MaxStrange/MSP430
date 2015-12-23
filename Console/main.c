#include <msp430.h>
#include "my_clock.h"
#include "my_uart.h"
#include "my_led.h"
#include "my_system.h"
#include "my_strings.h"
#include "console.h"


void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    clock_init();
    led_init();
    uart_init();
    _enable_interrupts();

    while (1)
    {
    	console_go();
    }
}
