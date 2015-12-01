/*
 * my_lcd.c
 *
 *  Created on: Nov 25, 2015
 *      Author: Max
 */
#include "my_lcd.h"


#define DEBUG_BOOK 0
#define DEBUG_MINE 1


void lcd_send(unsigned char data)
{
	LCD_DATA = ((data & 0xF0) >> 4);//send the upper four bits of the char
	pulse();
	LCD_DATA = (data & 0x0F);//send the lower four bits
	pulse();
}

void lcd_write_str(const char *str)
{
	LCD_ALERT_SEND_CHARACTER();
	while (*str)
	{
		lcd_send(*str++);
	}
}

void lcd_write_char(char ch)
{
	LCD_ALERT_SEND_CHARACTER();
	lcd_send(ch);
}

void lcd_goto(char x, char y)
{
	LCD_ALERT_SEND_COMMAND();
	if (x == 1)
		lcd_send(0x80 + (y - 1));//???????
	else
		lcd_send(0xC0 + (y - 1));//???????
}

/*
 * Initializes pin configuration for lcd module.
 */
void lcd_init(void)
{
	P1DIR |= (LCDPIN_RS | LCDPIN_E);
	P3DIR |= (LCDPIN_D4 | LCDPIN_D5 | LCDPIN_D6 | LCDPIN_D7);
}

/*
 *Initializes the actual lcd module.
 */
void lcd_start(void)
{
#if DEBUG_BOOK
	LCD_ALERT_SEND_COMMAND();

	delay_ms(40);
	lcd_send(0x30);			//0011 0000

	delay_ms(1);
	lcd_send(0x28);			//0010 1000

	delay_ms(1);
	lcd_send(0x28);			//0010 1000

	delay_ms(1);
	lcd_send(0x0C);			//0000 1100

	delay_ms(1);
	lcd_send(0x01);			//0000 0001

	delay_ms(2);
	lcd_send(0x06);			//0000 0110
#endif
#if DEBUG_MINE
	//delay at least 40ms after power rises above 2.7v before
	//we send any commands
	delay_ms(50);

	/*Now we want to initialize the LCD according to its datasheet
	specifications (page 46, figure 24, 4-bit interface
	initialization)
	*/

	send_4bits(0x03);
	delay_ms(5);

	send_4bits(0x03);
	delay_ms(5);

	send_4bits(0x03);
	__delay_cycles(150);

	send_4bits(0x02);

	/*
	 * From here on, we can now use the lcd_send() function
	 * to send 8 bit commands over the four bit interface.
	 */

	/*
	 * Finish initializing the LCD
	 * See page 46, figure 24, 4-bit interface
	 */

	delay_ms(1);
	lcd_send(0x28);//Set to have 2 lines of display - see p.29, table 8 (function set)

	delay_ms(1);
	lcd_send(0x08);//display off

	delay_ms(1);
	lcd_send(0x01);//clear display

	delay_ms(1);
	lcd_send(0x06);//have display shift left when adding chars - see p.26
#endif
}

/*
 * A function to send only the upper four bits of
 * an unsigned char.
 */
static void send_4bits(unsigned char to_send)
{
	LCD_DATA = ((0x0F) & to_send);
	pulse();
}

static void pulse(void)
{
	LCD_ADMIN |= LCDPIN_E;
	__delay_cycles(10);
	LCD_ADMIN &= ~LCDPIN_E;
	__delay_cycles(10);
}

