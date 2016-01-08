#ifndef RADIO_H_
#define RADIO_H_

#define CE_PIN 	(1 << 0)		//pin 1.0
#define IRQ_PIN	(1 << 1)		//pin 1.1


#define CMD_WRD_READ_RX_PAYLOAD			0x61
#define CMD_WRD_WRITE_TX_PAYLOAD		0xA0		//remember to write LSByte first (MSbit in each byte first)
#define CMD_WRD_FLUSH_TX				0xE1
#define CMD_WRD_FLUSH_RX				0xE2
#define CMD_WRD_REUSE_TX_PL				0xE3
#define CMD_WRD_READ_RX_PL_WID			0x60
#define CMD_WRD_WRITE_ACK_PAYLOAD_0		0xA8
#define CMD_WRD_WRITE_ACK_PAYLOAD_1		0xA9
#define CMD_WRD_WRITE_ACK_PAYLOAD_2		0xAA
#define CMD_WRD_WRITE_ACK_PAYLOAD_3		0xAB
#define CMD_WRD_WRITE_ACK_PAYLOAD_4		0xAC
#define CMD_WRD_WRITE_ACK_PAYLOAD_5		0xAD
#define CMD_WRD_WRITE_TX_PYLD_NO_ACK	0xB0
#define CMD_WRD_NOP						0xFF

//You can configure and control the radio by accessing the register map through SPI
#define CMD_WRD_READ_REGISTER(addr)		(0x00 | addr)
#define CMD_WRD_WRITE_TO_REGISTER(addr) (0x20 | addr)

//Pass these addresses AND NO OTHERS to the two register macros
//Writing other addresses may cause the radio to malfunction
#define REGISTER_ADDRESS_CONFIG			0x00
#define REGISTER_ADDRESS_EN_AUTO_ACK	0x01
#define REGISTER_ADDRESS_EN_RX_ADDR		0x02
#define REGISTER_ADDRESS_SETUP_ADD_WID	0x03
#define REGISTER_ADDRESS_SETUP_RETRANS	0x04
#define REGISTER_ADDRESS_RF_CHANNEL		0x05
#define REGISTER_ADDRESS_RF_SETUP		0x06
#define REGISTER_ADDRESS_STATUS			0x07
#define REGISTER_ADDRESS_OBSERVE_TX		0x08
#define REGISTER_ADDRESS_RCV_PWR_DTCTR	0x09
#define REGISTER_ADDRESS_RX_ADDR_P0		0x0A
#define REGISTER_ADDRESS_RX_ADDR_P1		0x0B
#define REGISTER_ADDRESS_RX_ADDR_P2		0x0C
#define REGISTER_ADDRESS_RX_ADDR_P3		0x0D
#define REGISTER_ADDRESS_RX_ADDR_P4		0x0E
#define REGISTER_ADDRESS_RX_ADDR_P5		0x0F
#define REGISTER_ADDRESS_TX_ADDR		0x10
#define REGISTER_ADDRESS_RX_PYLD_WID_P0	0x11
#define REGISTER_ADDRESS_RX_PYLD_WID_P1	0x12
#define REGISTER_ADDRESS_RX_PYLD_WID_P2	0x13
#define REGISTER_ADDRESS_RX_PYLD_WID_P3	0x14
#define REGISTER_ADDRESS_RX_PYLD_WID_P4	0x15
#define REGISTER_ADDRESS_RX_PYLD_WID_P5	0x16
#define REGISTER_ADDRESS_FIFO_STATUS	0x17
#define REGISTER_ADDRESS_EN_DYNMC_PYLD	0x1C
#define REGISTER_ADDRESS_FEATURE		0x1D

/*
 * To write to one of the registers, use the WRITE_TO_REGISTER
 * macro, passing in the address you want to write to, to get
 * the command word to write over SPI. Then write that word over
 * SPI. Then write the actual configuration byte for that register
 * over SPI.
 */

void radio_init(volatile int *which_led_is_on);
unsigned char radio_read(void);
void radio_write(unsigned char c);



#endif /* RADIO_H_ */
