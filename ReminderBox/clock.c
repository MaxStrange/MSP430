#include <msp430.h>

#include "clock.h"

/*
 * clock.c
 *
 *  Created on: Jan 26, 2016
 *      Author: Max
 */


static volatile unsigned long millis = 0;	//The number of milliseconds so far ticked					//at least 32 bits in size (so rollover at 4294967295)
static volatile unsigned long seconds = 0;	//The number of seconds so far ticked

/*
 * Sets the clock to 1MHz, with SMCLK and MCLK sourced from
 * the internal DCO.
 */
void clock_init(void)
{
	DCOCTL = CALDCO_8MHZ;		//set the DCO to 8MHz
	BCSCTL2 |= SELM0;			//set the MCLCK source as DCO
	BCSCTL2 &= ~SELS;			//set the SMCLK source as DCO
}

/*
 * Blocks for the given number of milliseconds.
 */
void clock_delay_ms(unsigned int to_delay)
{
	unsigned long until = millis + to_delay;

	while (millis < until)
		;//block until delay is over
}

/*
 * Gets the number of milliseconds since clock_init was called.
 */
unsigned long clock_get_ms(void)
{
	return millis;
}

/*
 * Gets the number of seconds since clock_init was called.
 */
unsigned long clock_get_seconds(void)
{
	return seconds;
}

/*
 * Starts a timer that ticks every millisecond.
 */
void clock_start_timer(void)
{
	TA0CTL |= (MC1 | TASSEL_2);	//up mode, SMCLK source

	TA0CCR0 = 1000;				//Compare for 1000

	TA0CCTL0 &= ~CCIFG;
	TA0CCTL0 |= CCIE;			//Enable capture compare interrupt
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
	millis++;

	if (millis == 4294967295UL)
		millis = 0;
	else if (millis % 1000 == 0)
		seconds++;

	TA0R = 0;				//Reset the value of the timer
	TA0CCTL0 &= ~CCIFG;		//Manually clear interrupt flag
}
