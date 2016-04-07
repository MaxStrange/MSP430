#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "menu.h"
#include "screen.h"
#include "led.h"
#include "clock.h"
#include "real_time_clock.h"
#include "strings.h"

#include "ui_view.h"


volatile menu_system_t *current_menu;

#define BUF_LEN 25
static char last_displayed_str[BUF_LEN];
volatile static bool *sleep;



/*
 * Static functions
 */

static void update_last_string(const char *str);
static void display_menu_item(const char *str);
static void display_time(void);


void ui_view_init(volatile bool *sleep, volatile menu_system_t *menu_ptr)
{
	/*
	 * Initialize LCD module
	 */
    lcd_pin_init();
    lcd_start();
    lcd_light_on();
    lcd_clear();

    /*
     * Initialize LEDs
     */
    led_init();
    led_off();

    current_menu = menu_ptr;
}

void ui_view_display(void)
{
    while (!*sleep)
    {
    	switch (current_menu->current_choice)
    	{
    	case DISPLAY_TIME:
    		display_menu_item("CURRENT TIME");
    		break;
    	case CHECK_DUE_DATES:
    		display_menu_item("CHECK COMING DUE");
    		break;
    	case PAY_BILL:
    		display_menu_item("PAY BILL");
    		break;
    	case ENTER_NEW_DATE:
    		display_menu_item("ENTER NEW BILL");
    		break;
    	case SET_TIME:
    		display_menu_item("SET CURRENT TIME");
    		break;
    	case CHOICE_DISPLAY_TIME:
    		display_time();
    		break;
    	default:
    		display_menu_item("Update view switch");
    		break;
    	}

    /*
   	 * 				-> user has the following options:
   	 * 					->Pay bill (after which, the LED(s) should go off and the memory should be checked to make
   	 * 						sure there isn't another one coming and if not, set up the next alarm.)
   	 * 					->Check coming due dates
   	 * 					->Enter new bill (after which, the date should be loaded into memory, then the alarms need to be updated
   	 * 						to make sure they are still the next bills.)
   	 */
    }
}
static void display_menu_item(const char *str)
{
	if (!strings_compare(str, last_displayed_str))
	{
		lcd_clear();
		lcd_write_str(str);
		update_last_string(str);
	}
}

static void display_time(void)
{
	static uint32_t seconds_last = 0;
	uint32_t seconds_since_turn_on = clock_get_seconds();

	uint8_t time_array[7];
	if (seconds_last != seconds_since_turn_on)
	{
		rtc_get_time(time_array);
		lcd_write_time(time_array[0], time_array[1], time_array[2], time_array[3], time_array[4], time_array[5], time_array[6]);
	}

	seconds_last = seconds_since_turn_on;
}

static void update_last_string(const char *str)
{
	unsigned int i = 0;
	for (i = 0; i < BUF_LEN; i++)
		last_displayed_str[i] = '\0';

	unsigned int j = 0;
	while (*str != '\0')
		last_displayed_str[j++] = *str++;
}




