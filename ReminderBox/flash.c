#include <msp430.h>

#include "spi.h"

#include "flash.h"


static address_t current_location = { 0x00, 0x00, 0x00 };



static void increment_current_location(void);


void flash_init(void)
{
	spi_init();
	spi_chip_enable_high();
}

/*
 * Reads the byte at wherever the current location is
 * looking. Then increments the current location.
 */
unsigned char flash_read_next_byte(void)
{
	unsigned char byte = flash_read_byte_at(&current_location);
	increment_current_location();
	return byte;
}

/*
 * Reads the byte at the given address, assuming the
 * address is valid. Returns the byte found.
 */
unsigned char flash_read_byte_at(const address_t * const address)
{
	spi_chip_enable_low();

	spi_tx_byte(READ);
	spi_tx_byte(address->msb);
	spi_tx_byte(address->middle);
	spi_tx_byte(address->lsb);
	unsigned char byte = spi_tx_byte(NOP);

	spi_chip_enable_high();


	return byte;
}

/*
 * Writes the given byte to wherever the working location
 * is pointed to. Then increments to the working address to
 * look at the next available location.
 */
void flash_write_next_byte(unsigned char to_write)
{
	flash_write_byte_at(&current_location, to_write);
	increment_current_location();
}

/*
 * Writes the given byte to the given address in memory.
 * Checks if the address is valid and only bothers to write if
 * valid.
 */
void flash_write_byte_at(const address_t * const address, unsigned char to_write)
{
	spi_chip_enable_low();
	spi_tx_byte(WREN);
	spi_chip_enable_high();

	__delay_cycles(15);				//give enough time for CE to change

	spi_chip_enable_low();
	spi_tx_byte(PP);
	spi_tx_byte(address->msb);
	spi_tx_byte(address->middle);
	spi_tx_byte(address->lsb);
	spi_tx_byte(to_write);
	spi_chip_enable_high();

	__delay_cycles(15);				//give enough time for CE to change

	spi_chip_enable_low();
	spi_tx_byte(WRDI);
	spi_chip_enable_high();
}



static void increment_current_location(void)
{
	current_location.lsb += 1;
	if (current_location.lsb == 0x00)
	{
		current_location.middle += 1;
		if (current_location.middle == 0x00)
		{
			current_location.msb += 1;
		}
	}
}

