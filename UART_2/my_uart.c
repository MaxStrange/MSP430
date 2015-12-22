#include <msp430.h>
#include "my_uart.h"

static volatile char tx_buffer[NUM_ELEMENTS_IN_TX_BUFFER];
static volatile unsigned int tx_read_index = 0;
static volatile unsigned int tx_write_index = 0;
static volatile int read_and_write_on_same_lap = 1;//boolean

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
}

/*
 * Writes the given NULL-TERMINATED string to
 * the UART. Does not block. Requires global interrupts
 * to be on.
 */
void uart_write(char *str)
{
	while (*str != '\0')
	{
		while (tx_write_index < NUM_ELEMENTS_IN_TX_BUFFER)
		{
			tx_buffer[tx_write_index] = *str++;
			tx_write_index++;
			if (*str == '\0')
				break;
		}
		if (tx_write_index >= NUM_ELEMENTS_IN_TX_BUFFER)
		{
			tx_write_index = 0;
			read_and_write_on_same_lap = 0;//false
		}
	}

	//enable the interrupt to send the data
	UCA0IFG |= UCTXIFG;
	UCA0IE |= UCTXIE;
}

static void init_pins(void)
{
	//UCA0 - see p.76 of device-specific datasheet
	P2DIR |= BIT0;
	P2SEL1 |= BIT0 + BIT1;
	P2SEL0 &= ~(BIT0 + BIT1);
}

/*
 * Reads the next byte out of the txbuf and loads it into
 * UCA0TXBUF register.
 */
static void send_next_byte(void)
{
	if ((tx_read_index >= tx_write_index) && (read_and_write_on_same_lap))
	{
		//queue is empty, disable TX interrupt until it gets some more data to send
		UCA0IE &= ~UCTXIE;
		UCA0IFG &= ~UCTXIFG;
		return;
	}

	UCA0TXBUF = tx_buffer[tx_read_index];
	tx_read_index++;

	if (tx_read_index >= NUM_ELEMENTS_IN_TX_BUFFER)
	{
		tx_read_index = 0;
		read_and_write_on_same_lap = 1;//true
	}
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	switch (__even_in_range(UCA0IV, 18))
	{
	case 0x00://Vector 0: No interrupt... not sure what that means
		break;

	case 0x02://UCRXIFG -- received some data in the rx buf
		break;

	case 0x04://UCTXIFG -- sent some data from the tx buf
		send_next_byte();
		break;

	case 0x06://UCSTTIFG -- start byte received
		break;

	case 0x08://UCTXCPTIFG -- sent all the data from the tx buf including a stop bit
		break;

	default:
		break;
	}
}
