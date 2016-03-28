#include "led.h"
#include <stdio.h>
#include <msp430.h>



void led_init(void)
{
	P1DIR |= LED_MAX | LED_MISH;
}

void led_blink(void)
{
	P1OUT ^= (LED_MAX | LED_MISH);
}

void led_on(void)
{
	P1OUT |= LED_MAX | LED_MISH;
}

void led_off(void)
{
	P1OUT &= ~(LED_MAX | LED_MISH);
}
