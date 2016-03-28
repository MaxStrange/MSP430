#include <msp430.h> 

#include "screen.h"
#include "real_time_clock.h"
#include "clock.h"
#include "I2C.h"
#include "led.h"
#include "memory.h"



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

    /*
     * Initialize LEDs
     */
    led_init();
    led_on();

    if (rtc_get_fault())
    {
    	lcd_clear();
    	lcd_write_str("Replace clock   battery.");
    	rtc_clear_fault();
    }
    else
    {
    	lcd_clear();
    	lcd_write_str("Waiting...");
    }


    /*
     * Initialize the clock - shouldn't have to do this more than once
     */
//    rtc_init();
//    rtc_set_time(31, 17, FRIDAY, 25, 3, 16);
//	  uint8_t minutes, uint8_t hours_24, e_day_of_week_t day, uint8_t date, uint8_t month, uint8_t year_since_2000






	uint32_t seconds_since_turn_on = 0;
    uint32_t seconds_last = 0;
    /*
     * Initialize the flash memory module
     */
    memory_init();
    //memory_debug_erase_section();
//
//    lcd_clear();
//    lcd_write_str("Writing: "); lcd_write_int(123); lcd_write_char(' '); lcd_write_int(322);
//    while (seconds_since_turn_on < 5)
//    {
//    	;
//    }
//
//    static uint16_t write_array[2];
//    write_array[0] = 123; write_array[1] = 322;
//    memory_write_words(write_array, 2);
//
//
//    static uint16_t read_array[2];
//    memory_read_words(MEM_ADDR_FIRST, read_array, 2);
//
//    lcd_clear();
//    lcd_write_str("Read: "); lcd_write_int(read_array[0]); lcd_write_char(' '); lcd_write_int(read_array[1]);


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





    	/*
    	 * Overall architecture:
    	 *
    	 * LPM
    	 * if (wake due to button press)
    	 * 		display user interface
    	 * 				-> user has the following options:
    	 * 					->Pay bill (after which, the LED(s) should go off and the memory should be checked to make
    	 * 						sure there isn't another one coming and if not, set up the next alarm.)
    	 * 					->Check coming due dates
    	 * 					->Enter new bill (after which, the date should be loaded into memory, then the alarms need to be updated
    	 * 						to make sure they are still the next bills.)
    	 * else
    	 * 		//woke up due to alarm from RTC module
    	 * 		check memory to find who's bill is coming due
    	 * 		light the appropriate LED
    	 */
    }

	return 0;
}
