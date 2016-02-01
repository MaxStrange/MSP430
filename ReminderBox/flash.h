#ifndef FLASH_H_
#define FLASH_H_

typedef struct
{
	unsigned char msb;
	unsigned char middle;
	unsigned char lsb;
} address_t;

void flash_init(void);
unsigned char flash_read_next_byte(void);
unsigned char flash_read_byte_at(const address_t * const address);
void flash_write_next_byte(unsigned char to_write);
void flash_write_byte_at(const address_t * const address, unsigned char to_write);
void flash_move_to_first_saved(void);//moves the pointer to the first saved byte
void flash_find_first_saved(address_t *address);//fills in the given address



#endif /* FLASH_H_ */
