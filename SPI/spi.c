#include <msp430.h>
#include "spi.h"

void spi_init(void)
{
	/*
	 * Initialize pin configurations
	 *
	 * See p.76 of device-specific datasheet
	 */
	P2SEL1 |= BIT0 | BIT1;				//pins P2.0 and P2.1 are MOSI and MISO respectively
	P2SEL0 &= ~(BIT0 | BIT1);

	P1SEL1 |= BIT4 | BIT5;				//pins P1.4 and P1.5 are STE and CLK respectively
	P1SEL0 &= ~(BIT4 | BIT5);




	/*
	 * Initialize the SPI module.
	 *
	 * 1. Enable software reset.
	 * 2. Clock mode 3, MSB first, 8-bit SPI master, three pin mode
	 * 3. Use SMCLK, keep SW reset
	 * 4. Low bit of UCBRx is 2
	 * 5. High bit of UCBRx is zero
	 * 6. f_SCL = SMCLK / 2 = ~600kHz
	 * 7. No modulation						??
	 * 8. Enable internal loopback
	 * 9. Clear sw reset, resume operation
	 * 10. Enable USCI0 RX interrupt
	 */

	UCA0CTLW0 |= UCSWRST;							//software reset
	UCA0CTLW0 |= UCMSB | UCMST | UCSYNC | UCCKPL;	//Inactive high, synchronous, master, msb first
	UCA0CTLW0 |= UCSSEL_2;							//use SMCLK
	UCA0BR0 |= 0x02;
	UCA0BR1 = 0;									//configure clock freq

	UCA0STATW |= UCLISTEN;							//enable loopback mode

	UCA0CTLW0 &= ~UCSWRST;							//disable software reset, enable operation

	UCA0IE |= UCRXIE;								//enable rx interrupt
}
