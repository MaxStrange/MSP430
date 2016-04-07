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

void led_on_max(void)
{
	P1OUT |= LED_MAX;
}

void led_on_mish(void)
{
	P1OUT |= LED_MISH;
}

void led_off_max(void)
{
	P1OUT &= ~LED_MAX;
}

void led_off_mish(void)
{
	P1OUT &= ~LED_MISH;
}
