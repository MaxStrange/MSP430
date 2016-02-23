#include <msp430.h>
#include <stdint.h>

#include "clock.h"
#include "screen.h"
#include "strings.h"


/*Function declarations - private helpers*/
static void send(unsigned char data);
static void send_4bits(unsigned char to_send);
static void pulse(void);
static void write_all_chars_in_str(volatile const char *str);


/*State*/
static volatile char i_as_str[6];


/*
 * Moves the cursor to the location specified.
 * Zero-based indexing is used.
 */
void lcd_goto(char x, char y)
{
	LCD_ALERT_SEND_COMMAND();
	if (y == 0)
		send(0x80 + x);
	else
		send(0xC0 + x);
}

/*
 * Initializes pin configuration for lcd module.
 */
void lcd_pin_init(void)
{
	P1DIR |= LCDPIN_LIGHT;
	P2DIR |= (LCDPIN_RS | LCDPIN_E);
	P2DIR |= (LCDPIN_D4 | LCDPIN_D5 | LCDPIN_D6 | LCDPIN_D7);
}

void lcd_light_on(void)
{
	P1OUT |= LCDPIN_LIGHT;
}

void led_light_off(void)
{
	P1OUT &= ~LCDPIN_LIGHT;
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
	//send(0x28);
	send(LCD_CMND_TWO_LINES);

	clock_delay_ms(1);
	send(LCD_CMND_DSPLY_OFF);

	clock_delay_ms(1);
	send(LCD_CMND_CLR_DSPLY);

	clock_delay_ms(1);
	send(LCD_CMND_SHIFT_MODE_LEFT);


	/*
	 * Not part of the initialization procedure,
	 * but want to initialize these options anyway.
	 */
	clock_delay_ms(1);
	send(LCD_CMND_DSPLY_ON_NO_BLINK);
}

void lcd_clear(void)
{
	LCD_ALERT_SEND_COMMAND();

	clock_delay_ms(1);
	send(LCD_CMND_DSPLY_OFF);

	clock_delay_ms(1);
	send(LCD_CMND_CLR_DSPLY);

	clock_delay_ms(1);
	send(LCD_CMND_SHIFT_MODE_LEFT);

	clock_delay_ms(1);
	send(LCD_CMND_DSPLY_ON_NO_BLINK);
}

void lcd_clear_and_write(const char *str)
{
	lcd_clear();
	lcd_goto(0, 0);
	lcd_write_str(str);
}

void lcd_write_char(char ch)
{
	LCD_ALERT_SEND_CHARACTER();
	send(ch);
}

void lcd_write_int(uint16_t i)
{
	strings_itoa(i, i_as_str);
	volatile char *str = i_as_str;
	write_all_chars_in_str(str);
}

void lcd_write_str(const char *str)
{
	lcd_clear();

	uint8_t len = strings_get_length(str);

	if (len <= 15)
	{
		write_all_chars_in_str(str);
	}
	else if (len <= 31)
	{
		static char buffer_right[16];
		static char buffer_left[16];

		//reinitialize the buffers
		uint8_t i = 0;
		for (i = 0; i < 16; i++)
		{
			buffer_left[i] = 0;
			buffer_right[i] = 0;
		}

		strings_split(15, str, buffer_left, buffer_right);
		write_all_chars_in_str(buffer_left);
		lcd_goto(0, 1);
		write_all_chars_in_str(buffer_right);
	}
	else
	{
		lcd_write_str("string is too long.");//TODO string is too long, scroll
	}
}


/*
 * Private methods
 */


static void send(unsigned char data)
{
	uint8_t e = LCD_ADMIN & LCDPIN_E;
	uint8_t rs = LCD_ADMIN & LCDPIN_RS;
	LCD_DATA = ((((data & 0xF0) >> 4) << 2) | e | rs);//send the upper four bits of the char
	pulse();
	LCD_DATA = (((data & 0x0F) << 2) | e | rs);//send the lower four bits
	pulse();
}

/*
 * A function to send only the lower four bits of
 * an unsigned char.
 */
static void send_4bits(unsigned char to_send)
{
	uint8_t e = LCD_ADMIN & LCDPIN_E;
	uint8_t rs = LCD_ADMIN & LCDPIN_RS;
	LCD_DATA = (((0x0F & to_send) << 2) | e | rs);
	pulse();
}

static void pulse(void)
{
	LCD_ADMIN |= LCDPIN_E;
	__delay_cycles(20);
	LCD_ADMIN &= ~LCDPIN_E;
	__delay_cycles(20);
}

static void write_all_chars_in_str(volatile const char *str)
{
	LCD_ALERT_SEND_CHARACTER();
	while (*str != '\0')
	{
		send(*str++);
	}
}
