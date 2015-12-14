#include <msp430.h> 
#include "my_lcd.h"
#include "my_system.h"

/*Function declarations*/
static void clock_config(void);
static void write_to_lcd(void);


static void clock_config(void)
{
	/*
	 * Turn off the watchdog timer
	 */
	WDTCTL = WDTPW | WDTHOLD;

	/*
	 * Initialize clock settings
	 */
	CSCTL0_H = 0xA5;                          // Unlock register
	CSCTL1 |= DCOFSEL0 + DCOFSEL1;            // Set max. DCO setting
	CSCTL2 = SELA_1 + SELS_3 + SELM_3;        // set ACLK = vlo; MCLK = DCO
	CSCTL3 = DIVA_0 + DIVS_0 + DIVM_3;        // set all dividers
	CSCTL0_H = 0x01;                          // Lock Register
}

static void write_to_lcd(void)
{
	static char * lcd_str = "hello";
	lcd_goto(0, 0);
	lcd_write_str(lcd_str);
}

/*
 * main.c
 */
int main(void)
{
	lcd_pin_init();
	clock_config();
	lcd_start();

	while (1)
	{
		write_to_lcd();
		delay_ms(1500);
	}

	return 0;
}
