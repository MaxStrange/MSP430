#ifndef SPI_H_
#define SPI_H_

#define MOSI	6
#define MISO	7
#define SCLK	5
#define	CE		3

void spi_init(void);
void spi_chip_enable_high(void);
void spi_chip_enable_low(void);
unsigned char spi_tx_byte(unsigned char byte);


#endif /* SPI_H_ */
