#include <msp430.h>

#include "spi.h"

#include "flash.h"


static address_t current_location;
static const address_t first_time_powered_on = { 0x00, 0x01, 0x02 };//TODO : change this to be a spot in memory where we can store whether or not this is the first time powered on


void flash_init(void)
{
	spi_init();
	spi_chip_enable_high();

	if (flash_read_byte_at(&first_time_powered_on))//should come out as 0xFF if it is the first time powered on
	{//if this is the first time powered on, initialize the memory with a marker and update the first time powered on location to be false
		flash_write_byte_at(&current_location, 0x00);
		flash_write_byte_at(&first_time_powered_on, 0x00);
	}
	else
	{//this is not the first time we have turned on, so there is useful memory somewhere; go find it
		flash_find_first_saved(&current_location);
	}
}

/*
 * Reads the byte at wherever the current location is
 * looking. Then increments the current location.
 */
unsigned char flash_read_next_byte(void)
{
	return 0x00;
}

/*
 * Reads the byte at the given address, assuming the
 * address is valid. Returns the byte found.
 */
unsigned char flash_read_byte_at(const address_t * const address)
{
	return 0x00;
}

/*
 * Writes the given byte to wherever the working location
 * is pointed to. Then increments to the working address to
 * look at the next available location.
 */
void flash_write_next_byte(unsigned char to_write)
{
}

/*
 * Writes the given byte to the given address in memory.
 * Checks if the address is valid and only bothers to write if
 * valid.
 */
void flash_write_byte_at(const address_t * const address, unsigned char to_write)
{
}

/*
 * Moves the current working address to the first useful location.
 */
void flash_move_to_first_saved(void)
{
}

/*
 * Fills in the given address with the first address found that contains useful data.
 */
void flash_find_first_saved(address_t *address)
{

}

