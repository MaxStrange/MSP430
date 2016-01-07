#include "my_led.h"
#include <stdio.h>
#include <msp430.h>



void led_init(void)
{
	PJDIR |= LED1 | LED2 | LED3 | LED4;
	P3DIR |= LED5 | LED6 | LED7 | LED8;
}

void led_blink(void)
{
	PJOUT ^= LED1 | LED2 | LED3 | LED4;
	P3OUT ^= LED5 | LED6 | LED7 | LED8;
}

void led_on(void)
{
	PJOUT |= LED1 | LED2 | LED3 | LED4;
	P3OUT |= LED5 | LED6 | LED7 | LED8;
}

void led_on_i(int i)
{
	led_off();

	switch (i)
	{
	case 0:
		break;
	case 1:
		PJOUT |= LED1;
		break;
	case 2:
		PJOUT |= LED2;
		break;
	case 3:
		PJOUT |= LED3;
		break;
	case 4:
		PJOUT |= LED4;
		break;
	case 5:
		P3OUT |= LED5;
		break;
	case 6:
		P3OUT |= LED6;
		break;
	case 7:
		P3OUT |= LED7;
		break;
	case 8:
		P3OUT |= LED8;
		break;
	default:
		break;
	}
}

void led_off(void)
{
	PJOUT &= 0xf0;
	P3OUT &= 0x0f;
}

