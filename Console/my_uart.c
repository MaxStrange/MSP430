#include <msp430.h>
#include "my_uart.h"
#include "circular_queue.h"


static volatile circular_queue tx_buffer;
static volatile circular_queue rx_buffer;
static volatile int rx_data_is_complete = 0;//bool
static volatile unsigned int num_chars_from_user = 0;

/*
 * Outputs an interface to the console. Then blocks until the user
 * inputs a response. Then places the response in the buffer with a nul
 * terminator and returns.
 */
void uart_get_console_input(char *buffer, unsigned int buffer_length)
{
	do
	{
		/*
		 * Reinitialize the rx queue - it may be full of characters that the user has dumped in with a command that was too long
		 */
		unsigned int i = 0;
		volatile char throw_away = 0;
		for (i = 0; i < num_chars_from_user; i++)
			throw_away = circular_queue_read_next_char(&rx_buffer);
		num_chars_from_user = 0;

		uart_write("Please enter a command: ");

		int rx_interrupt_already_enabled = (UCA0IE & UCRXIE);		//save the whether the rx interrupt is enabled or not (bool)

		UCA0IE |= UCRXIE;											//enable the rx interrupt
		while (!rx_data_is_complete)
			;														//hang on until you get a complete statement from the user
		if (!rx_interrupt_already_enabled)
			UCA0IE &= ~UCRXIE;										//put the rx interrupt back into its previous state

		rx_data_is_complete = 0;

	} while (num_chars_from_user >= buffer_length);//need one less character from user than buffer length so we can null terminate
	num_chars_from_user = 0;

	unsigned int i = 0;
	for (i = 0; i < buffer_length; i++)
	{
		buffer[i] = circular_queue_read_next_char(&rx_buffer);
		if (buffer[i] == '\n')
			break;
	}
	buffer[i] = '\0';//replace the last char with the nul
//	unsigned int last = (i + 1) >= buffer_length ? buffer_length - 1 : i + 1;
//	buffer[last] = '\0';
}

void uart_init(void)
{
	init_pins();
	init_state();

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
void uart_write(const char *str)
{
	while (*str != '\0')
	{
		circular_queue_write_char(&tx_buffer, *str++);
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
 * Initializes this module's internal state.
 */
static void init_state(void)
{
	circular_queue_construct(&tx_buffer);
	circular_queue_construct(&rx_buffer);
}

static inline void read_data_into_rx_buffer(void)
{
	unsigned char c = UCA0RXBUF;//reading from the buffer resets the interrupt flag
	circular_queue_write_char(&rx_buffer, c);
	rx_data_is_complete = (c == '\n');
	num_chars_from_user++;
}

/*
 * Reads the next byte out of the txbuf and loads it into
 * UCA0TXBUF register.
 */
static inline void send_next_byte(void)
{
	if (circular_queue_is_empty(&tx_buffer))
	{
		//Disable TX interrupt until it gets some more data to send
		UCA0IE &= ~UCTXIE;
		UCA0IFG &= ~UCTXIFG;
		return;
	}

	UCA0TXBUF = circular_queue_read_next_char(&tx_buffer);
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	switch (__even_in_range(UCA0IV, 18))
	{
	case 0x00://Vector 0: No interrupt... not sure what that means
		break;

	case 0x02://UCRXIFG -- received some data in the rx buf
		read_data_into_rx_buffer();
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
