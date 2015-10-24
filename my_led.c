#include "my_led.h"
#include <stdio.h>
#include <msp430.h>



void init_led(void)
{
	PJDIR |= LED1 | LED2 | LED3 | LED4;
	P3DIR |= LED5 | LED6 | LED7 | LED8;
}

void led_blink(void)
{
	PJOUT ^= LED1 | LED2 | LED3 | LED4;
	P3OUT ^= LED5 | LED6 | LED7 | LED8;
}

