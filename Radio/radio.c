#include <msp430.h>
#include "radio.h"
#include "spi.h"

void radio_init(void)
{
	/*
	 * Initialize underlying SPI module
	 */
	spi_init();

	/*
	 * Initialize pin configurations
	 */
	P1DIR |= CE_PIN;					//CE is output - must write it high to use rx or tx mode
	P1DIR &= ~IRQ_PIN;					//IRQ is input - reads low when an interrupt is pending from radio

	P1OUT &= ~CE_PIN;					//Make sure CE is low when not in use, since it is active high.
}

unsigned char radio_read(void)
{
	unsigned char data = UCB0RXBUF;
	return data;
}

void radio_write(unsigned char c)
{
	UCB0TXBUF = c;

	UCB0IE |= UCTXIE;
}

