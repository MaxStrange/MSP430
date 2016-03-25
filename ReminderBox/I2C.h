#ifndef I2C_H_
#define I2C_H_

//using UCB0
#define SCL		BIT6//1.6
#define SDA		BIT7//1.7



void i2c_init(void);
void i2c_write_byte_to_device(uint8_t address, uint8_t start_register_address, uint8_t *byte_array, uint8_t array_length);
void i2c_read_bytes_from_device(uint8_t address, uint8_t start_register_address, uint8_t *byte_array, uint8_t array_length);


#endif /* I2C_H_ */
