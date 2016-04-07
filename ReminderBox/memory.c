#include "memory.h"
/*
 * This module provides the functions for writing to and reading from the on-chip flash memory.
 * The writing occurrs in a queue, so that each byte is essentially only ever written once.
 * If/when the queue pointer wraps around to the beginning, the first sector of memory is erased
 * and each sector that the queue pointer reaches after that is erased before any writes to it happen.
 */

/*
 * The memory lay out is as follows:
 * The main memory starts at 0xFFBF and goes to 0xC000, so it has 16kB of memory.
 * It is divided into 32 segments of 512 bytes each: (0x0200 is 512)
 *
 * 0xFFBF
 * 0xFDBF
 * 0xFBBF
 * 0xF9BF
 * 0xF7BF
 * 0xF5BF
 * 0xF3BF
 * 0xF1BF
 *
 * 0xEFBF
 * 0xEDBF
 * .....
 * .....
 *
 * 0xDFBF
 * .....
 *
 * 0xCFBF
 * 0xCDBF
 * 0xCBBF
 * 0xC9BF
 * 0xC7BF
 * 0xC5BF
 * 0xC3BF
 * -------stop writing and return to the beginning
 * 0xC1BF
 */

static uint16_t *cur_word = (uint16_t *) MEM_ADDR_FIRST;
static bool erase_from_now_on = false;//whether to erase the next block or not every time before we reach the next section


static bool erase_section(void);


void memory_init(void)
{
	//our clock is 8MHz -> divide it down to 330kHz -> divide by 24
	FCTL2 = FWKEY | FSSEL0 | 0x017;
}

bool memory_read_words(uint16_t address, uint16_t *words, uint8_t word_array_length)
{
	if ((address > MEM_ADDR_FIRST) || (address < MEM_ADDR_LAST))
	{
		return false;
	}
	else
	{
		__disable_interrupt();

		uint16_t *word_pointer = (uint16_t *) address;

		unsigned int i = 0;
		for (i = 0; i < word_array_length; i++)
		{
			words[i] = *word_pointer--;

			if (word_pointer <= (uint16_t *) MEM_ADDR_LAST)
				word_pointer = (uint16_t *) MEM_ADDR_FIRST;
		}

		bool failed = FCTL3 & FAIL;
		uint16_t fctl3 = FCTL3;
		FCTL3 = FWKEY | (~(FAIL | LOCKA) & fctl3 & 0x00FF);

		__enable_interrupt();

		return !failed;
	}
}

bool memory_write_words(uint16_t *words, uint8_t word_array_length)
{
	__disable_interrupt();

	FCTL3 = FWKEY;                       // Clear Lock bit
	FCTL1 = FWKEY | WRT;                 // Set WRT bit for write operation

	int i = 0;
	for (i = 0; i < word_array_length; i++)
	{
		uint16_t to_write = words[i];
		*cur_word = to_write;
		cur_word--;
	}

	FCTL1 = FWKEY;                        // Clear WRT bit
	bool failed = FCTL3 & FAIL;
	FCTL3 = FWKEY | LOCK;                 // Set LOCK bit

	__enable_interrupt();

	return !failed;



//TODO Before this module can be used for realsies, you have to figure out the memory structure of the flash in this device, so we can wrap around and erase when necessary.



//	FCTL3 = FWKEY | LOCK;
//
//	unsigned int i = 0;
//	for (i = 0; i < word_array_length; i++)
//	{
//		*cur_word = words[i];
//		cur_word--;
//
//		bool word_is_in_new_section = (((uint16_t)cur_word - MEM_ADDR_LAST) % 512 == 0);
//		if (cur_word <= (uint16_t *) MEM_ADDR_LAST)
//		{
//			erase_from_now_on = true;
//			erase_section();
//			cur_word = (uint16_t *) MEM_ADDR_FIRST;
//		}
//		else if (word_is_in_new_section && erase_from_now_on)
//		{
//			erase_section();
//		}
//	}
//
//	bool failed = FCTL3 & FAIL;
//	uint16_t fctl3 = FCTL3;
//	fctl3 &= (0x00FF & ~LOCKA);//clear the fwkey value (which is not FWKEY)
//	fctl3 |= FWKEY;//set the fwkey
//	fctl3 &= ~(FAIL | LOCK | LOCKA);//clear the lock and fail bits but keep from writing to LOCKA (which changes its state when written to)
//	FCTL3 = fctl3;//write to the register

//	__enable_interrupt();

//	return failed;
}


static bool erase_section(void)
{
	__disable_interrupt();

	FCTL1 = FWKEY | ERASE;
	uint16_t fctl3 = FCTL3;
	FCTL3 = FWKEY | (~(LOCK | LOCKA) & fctl3 & 0x00FF);//Clear the LOCK bit, but don't write to the LOCKA bit (writing a one to it toggles it)

	*cur_word = 0;

	bool failed = FCTL3 & FAIL;
	FCTL3 = FWKEY | LOCK | (~(FAIL | LOCKA) & fctl3 & 0x00FF);//Reset the LOCK bit, clear the FAIL bit, and don't toggle the LOCKA bit

	__enable_interrupt();

	return !failed;
}




void memory_debug_erase_section(void)
{
	erase_section();
}




