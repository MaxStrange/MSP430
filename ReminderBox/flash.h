#ifndef FLASH_H_
#define FLASH_H_

/*
 * Command words
 */


/*
 * Configuration commands
 */
#define NOP		0x00	//NOP
#define RDSR	0x05	//Read status register
#define WRSR	0x01	//Write status register

/*
 * Read commands
 */
#define READ	0x03	//read
#define HS_READ	0x0B	//read high speed

/*
 * Write commands
 */
#define WREN	0x06	//write enable
#define WRDI	0x04	//write disable
#define PP		0x02	//page program

/*
 * Sleep
 */
#define DPD		0xB9	//deep power down mode
#define RDPD	0xAB	//release from deep power down



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



#endif /* FLASH_H_ */
