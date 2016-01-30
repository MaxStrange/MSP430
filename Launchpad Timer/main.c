#include <msp430.h> 

#include "clock.h"

#define LED1 (1 << 0)
#define LED2 (1 << 6)
#define TIME 100000

/*
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    clock_init();
    clock_start_timer();

    P1DIR |= LED1 | LED2;

    __enable_interrupt();

    while (1)
    {
    	unsigned long seconds = clock_get_seconds();

    	if (seconds % 2 == 0)
    		P1OUT |= (LED1 | LED2);
    	else
    		P1OUT &= ~(LED1 | LED2);
    }

	return 0;
}
