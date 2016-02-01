#ifndef LCD_H_
#define LCD_H_


/*Control Pins*/
#define LCDPIN_LIGHT BIT4	//P1.4
#define LCDPIN_RS BIT0		//P2.0
#define LCDPIN_E BIT1		//P2.1

/*Data bus Pins*/
#define LCDPIN_D4 BIT2		//P2.2
#define LCDPIN_D5 BIT3		//P2.3
#define LCDPIN_D6 BIT4		//P2.4
#define LCDPIN_D7 BIT5 		//P2.5

/*LCD Ports*/
#define LCD_PORT P2OUT

/*Macro functions used by the LCD module*/
#define LCD_ALERT_SEND_COMMAND() (LCD_PORT &= ~LCDPIN_RS)	//A low signal on pin 4 of the LCD means an instruction command is going to be sent
#define LCD_ALERT_SEND_CHARACTER() (LCD_PORT |= LCDPIN_RS)	//A high signal on pin 4 of the LCD means a character is going to be sent

/*Function declarations - API methods*/
void lcd_pin_init(void);
void lcd_start(void);
void lcd_backlight_off(void);
void lcd_backlight_on(void);
void lcd_write_str(const char *str);
void lcd_write_char(char ch);
void lcd_goto(char x, char y);

#endif /* LCD_H_ */
