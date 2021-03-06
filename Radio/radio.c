#include <msp430.h>
#include "radio.h"
#include "spi.h"
#include "my_led.h"

static volatile int *led;;

void radio_init(volatile int *which_led_is_on)
{
	led = which_led_is_on;

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

	/*
	 * Configure the radio's registers
	 *
	 * The radio seems to shift out the top bit (bit 7) first (from the status register)
	 */

	/*
	 * CONFIG regiser:
	 * Bit		Function
	 * 7		Reserved
	 * 6		Mask interrupt caused by rx - 1: interrupt disabled
	 * 5		Mask interrupt caused by tx - 1: interrupt disabled
	 * 4		Mask interrupt caused by max retransmits - 1: interrupt disabled
	 * 3		Enable CRC - forced high if one of the bits in EN_AA register is high
	 * 2		CRC encoding scheme - 0: 1 byte, 1: 2 bytes
	 * 1		Power up or power down - 0: Power down, 1: power up
	 * 0		RX/TX control - 0: PTX, 1: PRX
	 */
	static const unsigned char config_command = CMD_WRD_WRITE_TO_REGISTER(REGISTER_ADDRESS_CONFIG);
	static const unsigned char config_write = (1 << 4);
	radio_write(config_command);
	radio_write(config_write);

	/*
	 * EN_AA register: Enable Auto Ack
	 * Bit		Function
	 * 7:6		Reserved
	 * 5		Enable auto ack on pipe 5 - defaults to yes (1)
	 * 4		..
	 * 3		..
	 * 2		..
	 * 1		..
	 * 0		..
	 */
	//nothing to do here, since I want auto ack on each pipe

	/*
	 * EN_RXADDR: Enable RX address
	 * Bit		Function
	 * 7:6		Reserved
	 * 5		Enable data pipe 5 - defaults to no (0)
	 * 4		..
	 * 3		..
	 * 2		..
	 * 1		..
	 * 0		..
	 */
	static const unsigned char en_rx_addr_command = CMD_WRD_WRITE_TO_REGISTER(REGISTER_ADDRESS_EN_RX_ADDR);
	static const unsigned char en_rx_addr_write = (1 << 1) | (1 << 0);//enable pipes 0 and 1
	radio_write(en_rx_addr_command);
	radio_write(en_rx_addr_write);

	/*
	 * SETUP_AW: Setup of address widths
	 * Bit 		Function
	 * 7:2		Reserved
	 * 1:0		RX/TX address widths : 00: illegal, 01: 3 bytes, 10: 4 bytes, 11: 5 bytes
	 * LSByte is used if address width is below 5 bytes
	 * Default is 11 (5 bytes)
	 */
	//Nothing to do here, since 5 byte address width is fine

	/*
	 * SETUP_RETR: Setup of automatic retransmission
	 * Bit		Function
	 * 7:4		Auto retransmit delay: 0000: 250us, 0001: 500us, 0010: 750us ... 1111: 4000us
	 * Defaults to 250us (0000)
	 */
	//Nothing to do here, 250us is fine

	/*
	 * RF_CH: RF Channel
	 * Bit		Function
	 * 7		Reserved
	 * 6:0		Sets the frequency at which the radio operates
	 * Defaults to 000010
	 */
	//Nothing to do here

	/*
	 * RF_SETUP
	 * Bit		Function
	 * 7		Enable continuous carrier transmit when high (default 0)
	 * 6		Reserved
	 * 5		Set RF data rate to 250kbps (defaults to 0)
	 * 4		Only used in tests...
	 * 3		Select between the high speed data rates if bit 5 is 1: 0->1Mbps (default), 1->2Mbps
	 * 2:1		Set RF output power in TX mode: 00: -18dBm, 01: -12dBm, 10: -6dBm, 11: 0dBm (default)
	 * 0		Don't care
	 */
	//Nothing to do here

	/*
	 * STATUS	This register is shifted out whenever a command is being sent
	 * Bit		Function
	 * 7		Reserved
	 * 6		Data ready RX FIFO interrupt - asserted when new data arrives in RX FIFO. Set 1 to clear.
	 * 5		Data sent TX FIFO interrupt - asserted when packet transmitted on TX (and ACK is received if auto ack). Write 1 to clear.
	 * 4		Max retransmit interrupt ..
	 * 3:1		Data pipe number for the payload available for reading from RX FIFO: 110 is not used and 111 means RX FIFO is empty
	 * 0		TX full flag - 1 means TX FIFO is full
	 */
	//let's clear the interrupt bits, since on power on they should be zero and if not, they are certainly meaningless
	static const unsigned char status_addr_command = CMD_WRD_WRITE_TO_REGISTER(REGISTER_ADDRESS_STATUS);
	static const unsigned char status_addr_write = (1 << 6) | (1 << 5) | (1 << 4);
	radio_write(status_addr_command);
	radio_write(status_addr_write);

	/*
	 * Data pipe addresses for receiving
	 * Must be sent LSByte first, with MSbit in each byte first
	 */
	static const unsigned char rx_pipe_0_addr_command = CMD_WRD_WRITE_TO_REGISTER(REGISTER_ADDRESS_RX_ADDR_P0);
//	static const unsigned long rx_pipe_0_addr_write = 0x123456789A;//nrf READS it as: 0x9A78563412
	radio_write(rx_pipe_0_addr_command);
	radio_write(0x9A);
	radio_write(0x78);
	radio_write(0x56);
	radio_write(0x34);
	radio_write(0x12);

	static const unsigned char rx_pipe_1_addr_command = CMD_WRD_WRITE_TO_REGISTER(REGISTER_ADDRESS_RX_ADDR_P1);
//	static const unsigned long rx_pipe_1_addr_write = 0x123456789B;
	radio_write(rx_pipe_1_addr_command);
	radio_write(0x9B);//LSByte must be different between pipes
	radio_write(0x78);
	radio_write(0x56);
	radio_write(0x34);
	radio_write(0x12);

	/*
	 * RX_PW_Pn		RX payload width pipe n
	 * Bit		Function
	 * 7:6		Reserved
	 * 5:0		Number of bytes in RX payload on this pipe - 0 means pipe not used, all the way up to 32 bytes
	 * defaults to 0 (pipe not used)
	 */
	//let's do one byte on each pipe that we are using
	static const unsigned char rx_pipe_0_width_addr_command = CMD_WRD_WRITE_TO_REGISTER(REGISTER_ADDRESS_RX_PYLD_WID_P0);
	static const unsigned char rx_pipe_0_width_addr_write = 1;
	radio_write(rx_pipe_0_width_addr_command);
	radio_write(rx_pipe_0_width_addr_write);

	static const unsigned char rx_pipe_1_width_addr_command = CMD_WRD_WRITE_TO_REGISTER(REGISTER_ADDRESS_RX_PYLD_WID_P1);
	static const unsigned char rx_pipe_1_width_addr_write = 1;
	radio_write(rx_pipe_1_width_addr_command);
	radio_write(rx_pipe_1_width_addr_write);

	/*
	 * DYNPD	Enable dynamic payload length
	 * Bit		Function
	 * 7:6		Reserved
	 * 5		Enable dynamic payload length on data pipe 5 - requires EN_DPL (in feature register) and ENAA_P5 (in EN_AA register)
	 * 4		..
	 * 3		..
	 * 2		..
	 * 1		..
	 * 0		..
	 * Defaults to no (0).
	 */
	//Not sure why I would need this - check out the datasheet. But for testing,
	//I am only using one byte payloads anyway, so let's ignore it for now.

	/*
	 * FEATURE	Feature register
	 * Bit		Function
	 * 7:3		Reserved
	 * 2		EN_DPL enable dynamic payload length - defaults to 0 (no)
	 * 1		Enable ack payload... defaults to 0... not sure what it means
	 * 0		Enables the W_TX_PAYLOAD_NO_ACK command.. ugghh.... figure this out TODO
	 */
	//Really not clear if I need to enable w_tx_payload_no_ack or not. TODO


	//Now try writing some data

	//load in the payload into the TX FIFO
	radio_write(CMD_WRD_WRITE_TX_PAYLOAD);
	radio_write(0x13);//write the payload 0x13

	//assert CE, which should (since there is data in the TX FIFO),
	//set the mode to TX mode and have it send the packet
	P1OUT |= CE_PIN;
	_delay_cycles(600);
	P1OUT &= ~CE_PIN;
}

unsigned char radio_read(void)
{
	unsigned char data = UCB0RXBUF;
	return data;
}

void radio_write(unsigned char c)
{
	UCB0TXBUF = c;
	UCB0IFG |= UCTXIFG;
	UCB0IE |= UCTXIE;
}

static inline void handle_rx_data(void)
{
	char data = radio_read();

	(*led)++;

	if (*led > 8)
		*led = 0;

	led_on_i(*led);
}

#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
	switch (__even_in_range(UCB0IV, 18))
	{
	case 0x00://Vector 0: No interrupt... not sure what that means
		break;

	case 0x02://UCRXIFG -- received some data in the rx buf
		handle_rx_data();
		UCB0IFG &= ~UCRXIFG;
		break;

	case 0x04://UCTXIFG -- sent some data from the tx buf
		UCB0IE &= ~UCTXIE;
		UCB0IFG &= ~UCTXIFG;
		break;

	case 0x06://UCSTTIFG -- start byte received
		break;

	case 0x08://UCTXCPTIFG -- sent all the data from the tx buf including a stop bit
		break;

	default:
		break;
	}
}

