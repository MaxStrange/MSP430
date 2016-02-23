#include <msp430.h> 

#include "screen.h"
#include "real_time_clock.h"
#include "clock.h"



int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /*
     * Initialize clock module and interrupts (otherwise, clock won't work)
     */
	clock_init();
	clock_start_timer();
	__enable_interrupt();		//enable global interrupts

	/*
	 * Initialize LCD module (depends on clock module)
	 */
    lcd_pin_init();
    lcd_start();




    lcd_light_on();
    lcd_clear();
    lcd_write_str("Heaven is a place on earth.");

    while (1)
    {
    	;
    }

	return 0;
}
