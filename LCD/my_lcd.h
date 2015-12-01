/*
 * my_lcd.h
 *
 *  Created on: Nov 25, 2015
 *      Author: Max
 */

#ifndef MY_LCD_H_
#define MY_LCD_H_

#include <msp430.h>
#include "my_system.h"

#define LCDPIN_RS BIT6	//P1.6
#define LCDPIN_E BIT7	//P1.7

#define LCDPIN_D4 BIT0	//P3.0
#define LCDPIN_D5 BIT1	//P3.1
#define LCDPIN_D6 BIT2	//P3.2
#define LCDPIN_D7 BIT3 	//P3.3

#define LCD_ADMIN P1OUT
#define LCD_DATA P3OUT
#define LCD_ALERT_SEND_COMMAND() (LCD_ADMIN &= ~LCDPIN_RS)	//A low signal on pin 4 of the LCD means an instruction command is going to be sent
#define LCD_ALERT_SEND_CHARACTER() (LCD_ADMIN |= LCDPIN_RS)	//A high signal on pin 4 of the LCD means a character is going to be sent

void lcd_init(void);
void lcd_start(void);
void lcd_send(unsigned char data);
void lcd_write_str(const char *str);
void lcd_write_char(char ch);
void lcd_goto(char x, char y);

static void send_4bits(unsigned char to_send);
static void pulse(void);

#endif /* MY_LCD_H_ */
