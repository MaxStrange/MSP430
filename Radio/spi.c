#include <msp430.h>
#include "spi.h"

void spi_init(void)
{
	/*
	 * Initialize pin configurations
	 *
	 * See p.76 of device-specific datasheet
	 */
	P1SEL1 |= BIT6 | BIT7;				//pins P1.6 and P1.7 are MOSI and MISO respectively
	P1SEL0 &= ~(BIT6 | BIT7);

	P1SEL1 |= BIT3;						//pins P1.3 is STE
	P1SEL0 &= ~BIT3;

	P2SEL1 |= BIT2;						//pin P2.2 is CLK
	P2SEL0 &= ~BIT2;


	/*
	 * Initialize the SPI module.
	 *
	 * 1. Enable software reset.
	 * 2. Configure SPI
	 * 3. Clear sw reset, resume operation
	 * 4. Enable RX interrupt
	 */

	UCB0CTLW0 |= UCSWRST;							//software reset

	UCB0CTLW0 |= UCMSB | UCMST | UCSYNC | UCCKPL;	//Inactive high, synchronous, master, msb first
	UCB0CTLW0 |= UCSSEL_2;							//use SMCLK
	UCB0BR0 |= 0x02;
	UCB0BR1 = 0;									//configure clock freq
	UCB0STATW |= UCLISTEN;							//enable loopback mode

	UCB0CTLW0 &= ~UCSWRST;							//disable software reset, enable operation

	UCB0IE |= UCRXIE;								//enable rx interrupt
}
