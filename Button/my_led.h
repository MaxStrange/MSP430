#ifndef MY_LED_H_
#define MY_LED_H_


//LED 1 - 4 on portJ
#define LED1 0x01//0000 0001
#define LED2 0x02//0000 0010
#define LED3 0x04//0000 0100
#define LED4 0x08//0000 1000

//LED 5 - 8 on port3
#define LED5 0x10//0001 0000
#define LED6 0x20//0010 0000
#define LED7 0x40//0100 0000
#define LED8 0x80//1000 0000

void init_led(void);
void led_blink(void);
void led_off(void);
void led_on(void);


#endif /* MY_LED_H_ */
