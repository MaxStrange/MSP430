#ifndef MEMORY_H_
#define MEMORY_H_

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>


#define MEM_ADDR_FIRST	0xFFBF
#define MEM_ADDR_LAST	0xC3BF



/*
 * API methods
 */


void memory_init(void);

/*
 * Reads however many words from the memory starting at the given address.
 */
bool memory_read_words(uint16_t address, uint16_t *words, uint8_t byte_array_length);
/*
 * Writes however many words to the memory queue.
 */
bool memory_write_words(uint16_t *words, uint8_t byte_array_length);

void memory_debug_erase_section(void);


#endif /* MEMORY_H_ */
