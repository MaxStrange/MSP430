#include <msp430.h> 
#include "my_led.h"

#define BUTTON BIT0

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    //enable pullup resistor on the button pin
    P4REN |= BUTTON;
    P4OUT |= BUTTON;

    init_led();
    led_off();

    while (1)
    {
    	if ((P4IN & BUTTON) == 0x00)
    		led_on();
    	else
    		led_off();
    }

	return 0;
}
