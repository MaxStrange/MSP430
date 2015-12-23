#include <msp430.h>
#include "my_led.h"


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
