#include <msp430.h>
#include "spi.h"

void spi_init(void)
{
	/*
	 * Initialize pin configurations
	 *
	 * See p.44 of device-specific datasheet
	 */
	P1SEL |= SCLK;					//SPI SCLK
	P1SEL2 &= ~SCLK;

	P1SEL |= MISO;					//SPI SDO
	P1SEL2 &= ~MISO;

	P1SEL |= MOSI;					//SPI SDI
	P1SEL2 &= ~MOSI;

	P1DIR |= CE;					//SPI CE
	P1OUT |= CE;					//Active low, so set it high

	/*
	 * Initialize the SPI module.
	 *
	 * 1. Enable software reset.
	 * 2. Configure SPI
	 * 3. Clear sw reset, resume operation
	 * 4. Enable RX interrupt
	 */

	UCA0CTL1 |= UCSWRST;							//software reset

	UCA0CTL1 |= UCSSEL_3;							//Source clock is SMCLK
	UCA0CTL1 |= UCMSB | UCMST | UCSYNC | UCCKPL;	//Inactive high, synchronous, master, msb first, 3-pin mode
	UCA0BR0 |= 0x02;
	UCA0BR1 = 0;									//configure clock freq

	UCA0CTL1 &= ~UCSWRST;							//disable software reset, enable operation
}

void spi_chip_enable_high(void)
{
	P1OUT |= CE;
}

void spi_chip_enable_low(void)
{
	P1OUT &= ~CE;
}

/*
 * Blocks until received character from slave. Returns
 * that character.
 */
inline unsigned char spi_tx_byte(unsigned char byte)
{
	UCA0TXBUF = byte;

	while (!(IFG2 & UCA0TXIFG));

	return UCA0RXBUF;
}
