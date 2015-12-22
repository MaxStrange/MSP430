#include <msp430.h>
#include "my_clock.h"
#include "my_uart.h"
#include "my_led.h"
#include "my_system.h"

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    clock_init();
    led_init();
    uart_init();
    _enable_interrupts();

    while (1)
    {
    	led_toggle_all();
    	uart_write("hello!");
    	system_delay(100);
    }

    //TODO : build the send interrupt with a queue - when the queue is empty, turn off the interrupt. When you
    //put something in the queue, turn on the interrupt

    //TODO : build a console program

    //-Interrupts do not have to turn on other interrupts when done with their code - the processor does it for you
    //-General interrupt enable bit (GIE) turns on or off MASKABLE interrupts (peripheral pin interrupts), and each one can be turned on or off separately
    //-Interrupt latency is six cycles
    //-The instruction right after an enable interrupt instruction is always executed - so don't put enable followed immediately by disable
    //-Nesting is only available if you set the GIE inside an ISR. After the ISR returns, the settings switch back to whatever they were OUTSIDE the ISR.
    //-If you have turned on nesting, ANY interrupt (regardless of priorities) will interrupt an already going ISR
}
