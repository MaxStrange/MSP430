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

//    rtc_init();
//    rtc_set_time(31, 17, FRIDAY, 25, 3, 16);
//	uint8_t minutes, uint8_t hours_24, e_day_of_week_t day, uint8_t date, uint8_t month, uint8_t year_since_2000

    lcd_clear();
    lcd_write_str("Waiting...");

	uint32_t seconds_since_turn_on = 0;
    uint32_t seconds_last = 0;


//    rtc_set_alarm1(0, 43, 13, 25);



    while (1)
    {
    	seconds_since_turn_on = clock_get_seconds();

    	if (seconds_last != seconds_since_turn_on)
    	{
    		static uint8_t time_array[7];

    		rtc_get_time(time_array);
    		lcd_write_time(time_array[0], time_array[1], time_array[2], time_array[3], time_array[4], time_array[5], time_array[6]);
    	}

    	seconds_last = seconds_since_turn_on;
    }

	return 0;
}
