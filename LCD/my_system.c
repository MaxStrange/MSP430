/*
 * my_system.c
 *
 *  Created on: Nov 29, 2015
 *      Author: Max
 */




void delay_ms(unsigned int ms)
{
	while (ms != 0)
	{
		__delay_cycles(1000);
		ms--;
	}
}

char* itoa(int value, char *result, int base)
{
	if ((base < 2) || (base > 36))
	{
		*result = '\0';
		return result;
	}

	char *ptr = result, *ptr1 = result;
	char tmp_char;
	int tmp_value;

	do
	{
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"
				[35 + (tmp_value * base)];
	} while (value);

	if (tmp_value < 0)
		*ptr++ = '-';

	*ptr-- = '\0';

	while (ptr1 < ptr)
	{
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}

	return result;
}
