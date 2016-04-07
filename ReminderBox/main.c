#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "screen.h"
#include "real_time_clock.h"
#include "clock.h"
#include "I2C.h"
#include "led.h"
#include "memory.h"
#include "user_interface.h"


static void debug_memory_test(void);
static void debug_show_message(char *str, uint16_t *nums, uint16_t number_of_nums, uint16_t duration);


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /*
     * Initialize clock module and interrupts (otherwise, clock won't work). Most other modules depend on this one, so be
     * sure to initialize it first.
     */
	clock_init();
	clock_start_timer();
	__enable_interrupt();		//enable global interrupts

	/*
	 * Initialize user interface
	 */
	user_interface_init();


    /*
     * The RTC has already been initialized, so simply check if its battery has died. If so, let the user know it needs to
     * be replaced and the time needs to be reset.
     *
     * While debugging, even though the RTC doesn't lose power when you turn off the MSP430 (since it has its own battery),
     * it will insist that it has had a fault and therefore the battery should be replaced. This is not so. Ignore it.
     */
//    if (rtc_get_fault())
//    {
//    	debug_show_message("Replace RTC battery.", NULL, 0, 3);
//    	rtc_clear_fault();
//    	debug_show_message("Still bad", NULL, 0, 3);
//    }
//    else
//    {
//    	debug_show_message("Initializing system.", NULL, 0, 3);
//    }


    /*
     * Initialize the flash memory module
     */
    memory_init();


    /*
     * Memory test for debug purposes
     */
//    debug_memory_test();


    while (1)
    {
    	//LPM;
    	//if (wake up due to button press) :
    	user_interface_display();

    	/*
    	 * Overall architecture:
    	 *
    	 * LPM
    	 * if (wake due to button press)
    	 * 		display user interface
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
