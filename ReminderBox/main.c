#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>

#include "screen.h"
#include "real_time_clock.h"
#include "clock.h"
#include "I2C.h"
#include "led.h"
#include "memory.h"


static void debug_memory_test(void);
static void debug_show_message(char *str, uint16_t *nums, uint16_t number_of_nums, uint16_t duration);


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

	uint32_t seconds_since_turn_on = 0;
    uint32_t seconds_last = 0;
    /*
     * Initialize the flash memory module
     */
    memory_init();


    /*
     * Memory test for debug purposes
     */
    debug_memory_test();


    uint8_t time_array[7];
    while (1)
    {
    	seconds_since_turn_on = clock_get_seconds();

    	if (seconds_last != seconds_since_turn_on)
    	{
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


static void debug_memory_test(void)
{
	/*
	 * Establish arrays for test read and test write.
	 */

    uint16_t read_array[2];
    uint16_t write_array[2] = { (uint16_t)123 , (uint16_t)322 };

    /*
     * Read before memory has been erased.
     */

    bool read_worked = memory_read_words(MEM_ADDR_FIRST, read_array, 2);
    debug_show_message("Read:", read_array, 2, 3);

    /*
     * Erase memory segment.
     */

    memory_debug_erase_section();

    /*
     * Read after erase.
     */

    read_worked = memory_read_words(MEM_ADDR_FIRST, read_array, 2);
    debug_show_message("After erase:", read_array, 2, 3);

    /*
     * Write the data.
     */

    debug_show_message("Write:", write_array, 2, 3);
    bool write_worked = memory_write_words(write_array, 2);
    if (write_worked)
    	debug_show_message("Write success", write_array, 0, 3);
    else
    	debug_show_message("Write failed.", write_array, 0, 3);

    /*
     * Read the data after the write.
     */

    read_worked = memory_read_words(MEM_ADDR_FIRST, read_array, 2);
    debug_show_message("Read:", read_array, 2, 3);
}

static void debug_show_message(char *str, uint16_t *nums, uint16_t number_of_nums, uint16_t duration)
{
	uint32_t start = clock_get_seconds();

	lcd_clear();
	lcd_write_str(str);
	lcd_goto(0, 1);

	int i;
	for (i = 0; i < number_of_nums; i++)
	{
		lcd_write_int(nums[i]);
		lcd_write_char(' ');
	}

	uint32_t seconds = clock_get_seconds();
	while (seconds <= (start + duration))
	{
		seconds = clock_get_seconds();
	}
}
