#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "I2C.h"


static volatile bool nack_received_after_address_sent = false;

static volatile uint8_t write_data_array[25];
static volatile uint8_t write_data_index = 0;
static volatile uint8_t write_data_array_length = 0;
static volatile bool write_data_array_in_use = false;
static volatile bool emit_stop_condition = true;

static volatile uint8_t read_data_array[25];
static volatile uint8_t read_data_index = 0;
static volatile uint8_t read_data_array_length = 0;
static volatile bool read_data_array_in_use = false;


void i2c_init(void)
{
	P1DIR |= SCL;
	P1DIR &= ~SDA;

	//Toggle SCL line until SDA is observed to be high
	while (!(P1IN & SDA))
	{
		P1OUT ^= SCL;
	}


	/*
	 * The recommended USCI initialization process:
	 *
	 * 1. Set UCSWRST
	 * 2. Initialize all USCI registers
	 * 3. Configure ports
	 * 4. Clear UCSWRST
	 * 5. Enable interrupts if applicable
	 */

	UCB0CTL1 = UCSWRST;									//software reset
	UCB0CTL0 = UCMST | UCMODE_3;// | UCSYNC;			//Master mode, I2C mode
	UCB0CTL1 = UCSSEL_2 | UCSWRST;						//Source clock is SMCLK

	UCB0BR0 = 12;
	UCB0BR1 = 0;

	P1SEL |= SCL | SDA;
	P1SEL2 |= SCL | SDA;

	UCB0CTL1 &= ~UCSWRST;								//disable software reset, enable operation

	IE2 |= UCB0TXIE | UCB0RXIE;
	UCB0I2CIE |= UCNACKIE;
}

/*
 * Max array size = 24.
 *
 * TODO : Make a queue for the data to be sent from the I2C driver so this is not blocking
 *
 *
 * !!!IMPORTANT!!!
 * ALL interrupts OTHER THAN the I2C module's interrupts need to be disabled during a read and a write, otherwise
 * things become non-deterministic. If you ever add more interrupts, be sure to disable them here!
 */
void i2c_write_byte_to_device(uint8_t address, uint8_t start_register_address, uint8_t *byte_array, uint8_t array_length)
{
	bool clock_interrupt = TA0CCTL0 & CCIE;
	if (clock_interrupt)
		TA0CCTL0 &= ~CCIE;

	UCB0I2CSA = address >> 1;

	write_data_array_length = array_length + 1;
	uint8_t i = 0;
	write_data_array[0] = start_register_address;

	for (i = 1; i <= array_length; i++)
	{
		write_data_array[i] = byte_array[i - 1];
	}


	write_data_array_in_use = true;
	write_data_index = 0;
	UCB0CTL1 |= UCTR;
    UCB0CTL1 |= UCTXSTT;


	//interrupts should handle rest

    while (write_data_array_in_use)
    	;

    //clear the tx interrupt flag so it doesn't fire again
    IFG2 &= ~UCB0TXIFG;


    if (clock_interrupt)
    	TA0CCTL0 |= CCIE;
}

/*
 * Max array size = 24.
 *
 * Writes the given start address to the given device, then fills in the passed-in array with
 * whatever data comes back over the bus from the device.
 *
 *
 * !!!IMPORTANT!!!
 * ALL interrupts OTHER THAN the I2C module's interrupts need to be disabled during a read and a write, otherwise
 * things become non-deterministic. If you ever add more interrupts, be sure to disable them here!
 */
void i2c_read_bytes_from_device(uint8_t address, uint8_t start_register_address, uint8_t *byte_array, uint8_t array_length)
{
	bool clock_interrupt = TA0CCTL0 & CCIE;
	if (clock_interrupt)
		TA0CCTL0 &= ~CCIE;



	UCB0I2CSA = address >> 1;

	read_data_array_length = array_length;

	/*
	 * Set up where to start reading from
	 */
	emit_stop_condition = false;
	static uint8_t start_register_array[1];
	start_register_array[0] = start_register_address;
	i2c_write_byte_to_device(address, start_register_address, start_register_array, 0);
	emit_stop_condition = true;


	/*
	 * Read in the data the device gives
	 */
	IE2 &= ~(UCB0TXIE | UCB0RXIE);
	UCB0I2CIE &= ~UCNACKIE;

	read_data_array_in_use = true;
	read_data_index = 0;
	UCB0CTL1 |= UCTXSTT;
	UCB0CTL1 &= ~UCTR;

	IFG2 &= ~UCB0TXIFG;

	IE2 |= UCB0TXIE | UCB0RXIE;
	UCB0I2CIE |= UCNACKIE;


	while (read_data_array_in_use)
		;


	if (clock_interrupt)
		TA0CCTL0 |= CCIE;

	uint8_t j = 0;
	for (j = 0; j < array_length; j++)
	{
		byte_array[j] = read_data_array[j];
	}
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void state_isr(void)
{//This is the I2C's status change isr
	if (UCB0STAT & UCNACKIFG)
	{
		nack_received_after_address_sent = true;
		UCB0STAT &= ~UCNACKIFG;
	}
}


#pragma vector = USCIAB0TX_VECTOR
__interrupt void tx_rx_isr(void)
{//This is the I2C's tx AND rx isr
	if (UCB0CTL1 & UCTR)
	{
		//Sending data
		UCB0TXBUF = write_data_array[write_data_index];
		if (write_data_index >= write_data_array_length)
		{
			if (emit_stop_condition)
				UCB0CTL1 |= UCTXSTP;

			write_data_array_in_use = false;
		}
		else
		{
			write_data_index++;
		}
	}
	else
	{
		//Receiving data
		read_data_array[read_data_index] = UCB0RXBUF;
		if (read_data_index >= read_data_array_length)
		{
			UCB0CTL1 |= UCTXNACK;
			UCB0CTL1 |= UCTXSTP;
			read_data_array_in_use = false;
		}
		else
		{
			read_data_index++;
		}
	}

	IFG2 &= ~UCB0TXIFG;
}
