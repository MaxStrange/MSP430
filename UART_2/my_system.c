#include <msp430.h>
#include "my_system.h"
#include "my_clock.h"

void system_delay(unsigned long int milliseconds)
{
	//Empirically derived
	static const double MS_PER_CYCLE = 18.25 / MCLK_KHZ;

	volatile unsigned long len = (unsigned long)(((double)milliseconds / MS_PER_CYCLE) + 0.5);
	while (len > 0)
	{
		len--;
	}
}
