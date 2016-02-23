#include <msp430.h>
#include <stdint.h>

#include "I2C.h"


static volatile uint8_t nack_received_after_address_sent = 0;//boolean

void i2c_init(void)
{
	/*
	 * The recommended USCI initialization process:
	 *
	 * 1. Set UCSWRST
	 * 2. Initialize all USCI registers
	 * 3. Configure ports
	 * 4. Clear UCSWRST
	 * 5. Enable interrupts if applicable
	 */

	UCB0CTL1 |= UCSWRST;							//software reset
	UCB0CTL0 |= UCMST | UCMODE_3;					//Master mode, I2C mode
	UCB0CTL1 |= UCSSEL_3;							//Source clock is SMCLK

	P1SEL |= SCL | SDA;
	P1SEL2 |= SCL | SDA;

	UCB0CTL1 &= ~UCSWRST;							//disable software reset, enable operation

	UCB0I2CIE |= UCNACKIE;
}

void i2c_write_byte_to_device(uint8_t address, uint8_t start_register_address, uint8_t *byte_array, uint8_t array_length)
{
	/*
	 * After initialization, master transmitter mode is initiated by
	 * writing the desired slave address to the UCBxI2CSA
	 * register, selecting the size of the slave address with
	 * the UCSLA10 bit, setting UCTR for transmitter mode, and
	 * setting UCTXSTT to generate a START condition.
	 *
	 * To write data, simply put the byte to write into the
	 * UCBxTXBUF register and wait for it to be cleared out,
	 * at which point the UCB0TXIFG will be set.
	 *
	 * If no ACK is received after sending the slave address,
	 * the data byte is not written, and instead a flag,
	 * UCNACKIFG, is set. The master must then react with
	 * either a STOP condition (setting UCTXSTP), or by
	 * toggling the START condition.
	 */

	UCB0CTL1 &= ~UCTXSTP;
	UC0IFG &= ~UCB0TXIFG;

	do
	{
		nack_received_after_address_sent = 0;//false

		UCB0I2CSA = (address >> 1);	//TODO : make sure that this writes the right bits
		UCB0CTL1 |= UCTR;
		UCB0CTL1 |= UCTXSTT;
	} while (nack_received_after_address_sent);


	//write the first address and wait
	UCB0TXBUF = start_register_address;
	while (!(UC0IFG & UCB0TXIFG))
		;


	//now for each byte to write, write it, waiting at each write for the TXIFG to clear
	uint8_t i;
	for (i = 0; i < array_length; i++)
	{
		uint8_t byte = byte_array[i];

		UCB0TXBUF = byte;

		while (!(UC0IFG & UCB0TXIFG))
			;
	}

	//write stop condition
	UCB0CTL1 |= UCTXSTP;
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void state_isr(void)
{//This is the I2C's status change isr
	if (UCB0STAT & UCNACKIFG)
	{
		nack_received_after_address_sent = 1;//true
		UCB0STAT &= ~UCNACKIFG;
	}
}


#pragma vector = USCIAB0TX_VECTOR
__interrupt void tx_rx_isr(void)
{//This is the I2C's tx AND rx isr
}
