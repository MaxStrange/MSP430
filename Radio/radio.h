#ifndef RADIO_H_
#define RADIO_H_

#define CE_PIN 	(1 << 0)		//pin 1.0
#define IRQ_PIN	(1 << 1)		//pin 1.1


void radio_init(void);
unsigned char radio_read(void);
void radio_write(unsigned char c);



#endif /* RADIO_H_ */
