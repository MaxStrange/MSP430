#include <msp430.h>
#include "my_uart.h"

void uart_init(void)
{
	init_pins();
    /*
     * Setting a baud rate:
     *
     * According to p.488 of the user guide:
     *
     * To calculate the correct settings for the baudrate:
     * 1. Calculate N = f_brclk / baudrate
     * 		if N > 16, go to step 3, otherwise, go to step 2
     * 2. OS16 = 0, UCBRx = INT(N) (go to step 4)
     * 3. OS16 = 1, UCBRx = INT(N/16), UCBRFx = INT( [ (N/16) - INT(N/16) ] * 16 )
     * 4. UCBRSx can be found by looking up the fractional part of N (that is, N - INT(N)) in the table
     * 5. If OS16 = 0 was chosen, a detailed error calculation is recommended
	 *
     * Let's set up a baud rate of 9600.
     *
     * First, find the f_brclk. We could use the internal DCO, though this is fairly inaccurate. It defaults to 1 MHz.
     * Otherwise, we could use the on-board crystal (32.768 kHz).
     *
     * Let's use the crystal.
     *
     * N = 32768 / 9600 = 3.41333, so:
     * OS16 = 0; UCBR0 = 3; and according to the table, UCBRS0 = 0x92;
	 *
     * According to p.479 of the user guide:
     *
     * The recommended eUSCI_A initialization/reconfiguration
     * process is:
     * 1. Set UCSWRST bit (to avoid unpredictable behavior while configuring the USCI)
     * 2. Initialize all eUSCI_A registers including UCAxCTL1
     * 3. Configure ports
     * 4. Clear UCSWRST bit
     * 5. Enable interrupts via UCRXIE or UCTXIE
     */
    UCA0CTL1 |= UCSWRST;

	// Configure UART 0
	UCA0CTL1 |= UCSSEL_1;	//set the baud rate clock source to be ACLK
	UCA0BR0 = 3;			//clock prescaler
	UCA0MCTLW |= 0x9200;	//set the whole register in one instruction

    UCA0CTL1 &= ~UCSWRST;

    //Now enable interrupts if you want to
    //	TODO
}

void uart_write(char *str)
{
	while (*str != '\0')
	{
		while (!UCTXIFG)
			;

		UCA0TXBUF = *str++;
	}
}

static void init_pins(void)
{
	//UCA0 - see p.76 of device-specific datasheet
	P2SEL1 |= BIT0 + BIT1;
	P2SEL0 &= ~(BIT0 + BIT1);
}
