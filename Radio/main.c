#include <msp430.h> 
#include "my_led.h"
#include "my_button.h"
#include "radio.h"

static volatile int which_led_is_on = 0;

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /*
     * Initialize modules
     */
    led_init();
    button_init();
    radio_init(&which_led_is_on);


    /*
     * Initialize state
     */
    led_off();
    _enable_interrupts();//globally enable interrupts


    /*
     * Loop
     */
    while (1)
    {
    	;//everything is taken care of using interrupts
    }

	return 0;
}

#pragma vector=PORT4_VECTOR
__interrupt void PORT4_ISR(void)
{
	//debounce
	__delay_cycles(1500);
	if (!BUTTON_PRESSED)
		return;

	radio_write(which_led_is_on);

	P4IFG = 0x00;//Make sure you clear the interrupt flag before you return
}
