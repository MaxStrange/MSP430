#ifndef SPI_H_
#define SPI_H_

#define MOSI	BIT2		//P1.2
#define MISO	BIT1		//P1.1
#define SCLK	BIT4		//P1.4
#define	CE		BIT3		//P1.3


void spi_init(void);
void spi_chip_enable_high(void);
void spi_chip_enable_low(void);
unsigned char spi_tx_byte(unsigned char byte);


#endif /* SPI_H_ */
