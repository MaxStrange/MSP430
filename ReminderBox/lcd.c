#include <msp430.h>

#include "clock.h"

#include "lcd.h"

static void send(unsigned char data);
static void send_4bits(unsigned char to_send);
static void pulse(void);

/*
 * Moves the cursor to the location specified.
 * Zero-based indexing is used.
 */
void lcd_goto(char x, char y)
{
	LCD_ALERT_SEND_COMMAND();
	if (x == 0)
		send(0x80 + y);
	else
		send(0xC0 + y);
}

/*
 * Initializes pin configuration for lcd module.
 */
void lcd_pin_init(void)
{
	P1DIR |= LCDPIN_LIGHT;
	P1OUT &= ~LCDPIN_LIGHT;//active high, so keep low when not in use

	P2DIR |= (LCDPIN_RS | LCDPIN_E);
	P2DIR |= (LCDPIN_D4 | LCDPIN_D5 | LCDPIN_D6 | LCDPIN_D7);
}

/*
 *Initializes the actual lcd module.
 */
void lcd_start(void)
{
	LCD_ALERT_SEND_COMMAND();

	//delay at least 40ms after power rises above 2.7v before
	//we send any commands
	clock_delay_ms(50);

	/*Now we want to initialize the LCD according to its datasheet
	specifications (page 46, figure 24, 4-bit interface
	initialization)
	*/

	send_4bits(0x03);
	clock_delay_ms(5);

	send_4bits(0x03);
	clock_delay_ms(5);

	send_4bits(0x03);
	__delay_cycles(150);

	send_4bits(0x02);

	/*
	 * From here on, we can now use the send() function
	 * to send 8 bit commands over the four bit interface.
	 */

	/*
	 * Finish initializing the LCD
	 * See page 46, figure 24, 4-bit interface
	 */

	clock_delay_ms(1);
	send(0x28);//Set to have 2 lines of display - see p.29, table 8 (function set)

	clock_delay_ms(1);
	send(0x08);//display off

	clock_delay_ms(1);
	send(0x01);//clear display

	clock_delay_ms(1);
	send(0x06);//have display shift left when adding chars - see p.26

	clock_delay_ms(1);
	send(0x0C);//Display on, no blink
}

void lcd_backlight_off(void)
{
	P1OUT &= ~LCDPIN_LIGHT;
}

void lcd_backlight_on(void)
{
	P1OUT |= LCDPIN_LIGHT;
}

void lcd_write_char(char ch)
{
	LCD_ALERT_SEND_CHARACTER();
	send(ch);
}

void lcd_write_str(const char *str)
{
	LCD_ALERT_SEND_CHARACTER();
	while (*str != '\0')
	{
		send(*str++);
	}
}


/*
 * Private methods
 */


static void send(unsigned char data)
{
	LCD_PORT = ((data & 0xF0) >> 4);//send the upper four bits of the char
	pulse();
	LCD_PORT = (data & 0x0F);//send the lower four bits
	pulse();
}

/*
 * A function to send only the lower four bits of
 * an unsigned char.
 */
static void send_4bits(unsigned char to_send)
{
	LCD_PORT = (0x0F & to_send);
	pulse();
}

static void pulse(void)
{
	LCD_PORT |= LCDPIN_E;
	__delay_cycles(15);
	LCD_PORT &= ~LCDPIN_E;
	__delay_cycles(15);
}

