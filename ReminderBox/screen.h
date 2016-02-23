#ifndef SCREEN_H_
#define SCREEN_H_

#include <stdint.h>

/*Control Pins*/
#define LCDPIN_LIGHT	BIT5	//P1.5
#define LCDPIN_RS 		BIT0	//P2.0
#define LCDPIN_E 		BIT1	//P2.1

/*Data bus Pins*/
#define LCDPIN_D4 	BIT2	//P2.2
#define LCDPIN_D5 	BIT3	//P2.3
#define LCDPIN_D6 	BIT4	//P2.4
#define LCDPIN_D7 	BIT5 	//P2.5

/*LCD Ports*/
#define LCD_ADMIN 	P2OUT
#define LCD_DATA 	P2OUT

/*Command Codes*/
#define LCD_CMND_CLR_DSPLY          0x01
#define LCD_CMND_RTRN_HOME          0x02
#define LCD_CMND_SHIFT_MODE_LEFT    0x06
#define LCD_CMND_DSPLY_OFF          0x08
#define LCD_CMND_DSPLY_ON_NO_BLINK  0x0C
#define LCD_CMND_TWO_LINES          0x28

/*Macro functions used by the LCD module*/
#define LCD_ALERT_SEND_COMMAND() (LCD_ADMIN &= ~LCDPIN_RS)	//A low signal on RS of the LCD means an instruction command is going to be sent
#define LCD_ALERT_SEND_CHARACTER() (LCD_ADMIN |= LCDPIN_RS)	//A high signal on RS of the LCD means a character is going to be sent

/*Function declarations - API methods*/
void lcd_pin_init(void);
void lcd_start(void);
void lcd_clear(void);
void lcd_clear_and_write(const char *str);
void lcd_goto(char x, char y);
void lcd_light_on(void);
void lcd_light_off(void);
void lcd_write_int(uint16_t i);
void lcd_write_str(const char *str);
void lcd_write_char(char ch);


#endif /* SCREEN_H_ */
