#include <msp430.h>
#include "my_strings.h"

/*
 * Returns 1 if strings are equal, 0 otherwise.
 */
int strings_compare(const char *str1, const char *str2)//bool
{
	while ((*str1 != '\0') && (*str2 != '\0'))
	{
		if (*str1++ != *str2++)
			return 0;//false
	}

	return ((*str1 == '\0') && (*str2 == '\0'));//if not both at end, then strings aren't equal
}
