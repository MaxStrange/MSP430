#include <msp430.h> 

#include "screen.h"
#include "real_time_clock.h"
#include "clock.h"
#include "I2C.h"



int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /*
     * Initialize clock module and interrupts (otherwise, clock won't work)
     */
//	clock_init();
//	clock_start_timer();
	__enable_interrupt();		//enable global interrupts

	/*
	 * Initialize LCD module (depends on clock module)
	 */
//    lcd_pin_init();
//    lcd_start();
//
//    lcd_light_on();
//    lcd_clear();
//    lcd_write_str("Why did you eat my fries?");

    i2c_init();

    rtc_set_time(45, 14, TUESDAY, 23, 2, 16);
//	uint8_t minutes, uint8_t hours_24, e_day_of_week_t day, uint8_t date, uint8_t month, uint8_t year_since_2000

    while (1)
    {
    	;
    }

	return 0;
}
