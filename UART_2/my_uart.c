#include <msp430.h>
#include "my_uart.h"

void uart_init(void)
{
	init_pins();
    /*
     * Setting a baud rate found on p.488 of user guide.
     * Initializing the UART is on p.479 of user guide.
     */


	UCA0CTL1 |= UCSWRST;

	/*
	 * The following code block sets the clock source as the ACLK which should be running
	 * at 32.678 kHz.
	 *
	 * UCOS16 = 0; UCA0BR0 = 3; UCA0BRF0 = unchanged; UCA0BRS0 = 0x92
	 *
	 * TODO : THIS DOESN'T WORK. PERHAPS THE CRYSTAL IS NOT WORKING PROPERLY OR YOU
	 * ARE MISUNDERSTANDING HOW TO SOURCE FROM THE CRYSTAL.
	 */


//	UCA0CTL1 |= UCSSEL_1;	//set the baud rate clock source to be ACLK
//	UCA0BR0 = 3;			//clock prescaler
//	UCA0BR1 = 0;
//	UCA0MCTLW |= 0x9200;	//set the whole register in one instruction

	/*
	 * The following code block sets the clock source as the SMCLK which should be running
	 * at 1 MHz
	 *
	 * UCOS16 = 1; UCA0BR0 = 6; UCA0BRF0 = 8; UCA0BRS0 = 0x20; <-- should probably be 0x11
	 */

	UCA0CTL1 |= UCSSEL_2;	//set the baud rate clock source to be SMCLK
	UCA0BR0 = 6;			//clock prescaler
	UCA0BR1 = 0;
	UCA0MCTLW |= UCOS16;
	UCA0MCTLW |= 0x1180;

	UCA0CTL1 &= ~UCSWRST;


    //Now enable interrupts if you want to
    //	TODO
}

void uart_write(char *str)
{
	while (*str != '\0')
	{
		while (!(UCA0IFG & UCTXIFG))
			;

		UCA0TXBUF = *str++;
	}
}

static void init_pins(void)
{
	//UCA0 - see p.76 of device-specific datasheet
	P2DIR |= BIT0;
	P2SEL1 |= BIT0 + BIT1;
	P2SEL0 &= ~(BIT0 + BIT1);
}
