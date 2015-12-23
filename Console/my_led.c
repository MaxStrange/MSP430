#include <msp430.h>
#include "my_led.h"
#include "my_system.h"


void led_clear_all(void)
{
	PJOUT &= ~(LED1 | LED2 | LED3 | LED4);
	P3OUT &= ~(LED5 | LED6 | LED7 | LED8);
}

void led_init(void)
{
	PJDIR |= LED1 | LED2 | LED3 | LED4;
	P3DIR |= LED5 | LED6 | LED7 | LED8;
}

void led_pattern_blink(void)
{
	PJOUT |= LED1 | LED2 | LED3 | LED4;
	led_toggle_all();
	system_delay(250);
	led_toggle_all();
	system_delay(250);
	P3OUT |= LED5 | LED6 | LED7 | LED8;
	led_toggle_all();
}

void led_pattern_dance(void)
{
	led_toggle_led(LED1);
	led_toggle_led(LED8);
	system_delay(100);
	led_toggle_led(LED2);
	led_toggle_led(LED7);
	system_delay(200);
	led_toggle_led(LED3);
	led_toggle_led(LED6);
	system_delay(100);
	led_toggle_led(LED4);
	led_toggle_led(LED5);
	system_delay(200);
	led_toggle_led(LED5);
	led_toggle_led(LED4);
	system_delay(200);
	led_toggle_led(LED6);
	led_toggle_led(LED3);
	system_delay(100);
	led_toggle_led(LED7);
	led_toggle_led(LED2);
	system_delay(200);
	led_toggle_led(LED8);
	led_toggle_led(LED1);
	system_delay(100);
	led_toggle_led(LED3);
	led_toggle_led(LED6);
	system_delay(100);
	led_toggle_led(LED4);
	led_toggle_led(LED5);
	led_toggle_led(LED7);
	led_toggle_led(LED3);
	system_delay(100);
	led_toggle_led(LED7);
	led_toggle_led(LED2);
	system_delay(200);
	led_toggle_led(LED8);
	led_toggle_led(LED1);
	system_delay(100);
	led_pattern_loading_bar();
	led_toggle_all();
	system_delay(200);
	led_toggle_all();
	system_delay(100);
	led_clear_all();
}

void led_pattern_loading_bar(void)
{
	led_toggle_led(LED1);
	system_delay(100);
	led_toggle_led(LED2);
	system_delay(100);
	led_toggle_led(LED3);
	system_delay(100);
	led_toggle_led(LED4);
	system_delay(100);
	led_toggle_led(LED5);
	system_delay(100);
	led_toggle_led(LED6);
	system_delay(100);
	led_toggle_led(LED7);
	system_delay(100);
	led_toggle_led(LED8);
	system_delay(100);
}

void led_toggle_all(void)
{
	PJOUT ^= LED1 | LED2 | LED3 | LED4;
	P3OUT ^= LED5 | LED6 | LED7 | LED8;
}

void led_toggle_led(unsigned char led)
{
	if (led > LED4)
		P3OUT ^= led;
	else
		PJOUT ^= led;
}

void led_set_all(void)
{
	PJOUT |= LED1 | LED2 | LED3 | LED4;
	P3OUT |= LED5 | LED6 | LED7 | LED8;
}
