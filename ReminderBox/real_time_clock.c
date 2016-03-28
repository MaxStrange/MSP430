#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "I2C.h"

#include "real_time_clock.h"

static bool alarm1_set = false;
static bool alarm2_set = false;

static uint8_t itobcd(uint8_t val);
static uint8_t hours_to_bcd(uint8_t hours);
static void set_bit(uint8_t register_addr, uint8_t to_set);
static void clear_bit(uint8_t register_addr, uint8_t to_clear);
static uint8_t read_byte(uint8_t byte_address);


void rtc_init(void)
{
	set_bit(ADDR_CONTROL, CONTROL_BITS_RS2);
	set_bit(ADDR_CONTROL, CONTROL_BITS_RS1);
	set_bit(ADDR_CONTROL, CONTROL_BITS_INTCN);

	clear_bit(ADDR_CONTROL, CONTROL_BITS_ALARM1);
	clear_bit(ADDR_CONTROL, CONTROL_BITS_ALARM2);
	clear_bit(ADDR_CONTROL, CONTROL_BITS_BBSQW);
	clear_bit(ADDR_CONTROL, CONTROL_BITS_CONV);
	clear_bit(ADDR_CONTROL, CONTROL_BITS_EOSC);
}

void rtc_set_time(uint8_t minutes, uint8_t hours_24, e_day_of_week_t day, uint8_t date, uint8_t month, uint8_t year_since_2000)
{
	static uint8_t data_array[7];
	data_array[0] = (uint8_t)0;//seconds
	data_array[1] = itobcd(minutes);
	data_array[2] = hours_to_bcd(hours_24);
	data_array[3] = (uint8_t)day;
	data_array[4] = itobcd(date);
	data_array[5] = itobcd(month);
	data_array[6] = itobcd(year_since_2000);

	i2c_write_byte_to_device(MODULE_ADDRESS, ADDR_TIME_SECONDS, data_array, 7);
	rtc_init();
}

/*
 * Fills the array with:
 * 0->SECONDS (0 TO 59)
 * 1->MINUTES (0 TO 59)
 * 2->HOURS (0 TO 23)
 * 3->DAY (1 SUNDAY... 7 SATURDAY)
 * 4->DATE (1 TO 31)
 * 5->MONTH (1 TO 12)
 * 6->YEAR SINCE 2000
 */
void rtc_get_time(uint8_t *data_array_len_7)
{
	i2c_read_bytes_from_device(MODULE_ADDRESS, ADDR_TIME_SECONDS, data_array_len_7, 7);

	int i = 0;
	for (i = 0; i < 7; i++)
	{
		bool convert = false;
		switch (i)
		{
		case 0:
			//seconds
		case 1:
			//minutes
		case 2:
			//hours
			convert = true;
			break;
		case 3:
			//day
			break;
		case 4:
			//date
		case 5:
			//month
		case 6:
			//year
			convert = true;
			break;
		}

		if (convert)
		{
			uint8_t time = data_array_len_7[i];
			data_array_len_7[i] = (((time & 0xF0) >> 4) * 10) + (time & 0x0F);
		}
	}
}

void rtc_set_alarm1(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t date)
{
	static uint8_t alarm_time[4];
	alarm_time[0] = itobcd(seconds);
	alarm_time[1] = itobcd(minutes);
	alarm_time[2] = hours_to_bcd(hours);
	alarm_time[3] = itobcd(date);

	i2c_write_byte_to_device(MODULE_ADDRESS, ADDR_ALARM_SECONDS, alarm_time, 4);
	rtc_init();
	set_bit(ADDR_CONTROL, CONTROL_BITS_ALARM1);

	alarm1_set = true;
}

void rtc_set_alarm2(uint8_t minutes, uint8_t hours, uint8_t date)
{
	static uint8_t alarm_time[3];
	alarm_time[0] = itobcd(minutes);
	alarm_time[1] = hours_to_bcd(hours);
	alarm_time[2] = itobcd(date);

	i2c_write_byte_to_device(MODULE_ADDRESS, ADDR_ALARM2_MINUTES, alarm_time, 3);
	rtc_init();
	set_bit(ADDR_CONTROL, CONTROL_BITS_ALARM2);

	alarm2_set = true;
}

inline bool rtc_is_alarm1_set()
{
	return alarm1_set;
}

inline bool rtc_is_alarm2_set()
{
	return alarm2_set;
}

/*
 * Returns true if a fault has occurred - most likely indicates power failure at
 * some point, but either way, it means the clock needs to be reset.
 */
inline bool rtc_get_fault()
{
	return (read_byte(ADDR_STATUS) & STATUS_BITS_OSF);
}

/*
 * Clears the oscillator fault bit in the status register.
 */
inline void rtc_clear_fault()
{
	clear_bit(ADDR_STATUS, STATUS_BITS_OSF);
}

/*
 * Convert an integer to binary coded decimal. e.g: 34 -> 0x34
 */
static uint8_t itobcd(uint8_t val)
{
	uint8_t tens = val / 10;
	uint8_t ones = val - (tens * 10);

	return ((tens << 4) | ones);
}

static uint8_t hours_to_bcd(uint8_t hours)
{
	uint8_t hours_20s = (hours > 19) ? 1 : 0;
	uint8_t hours_10s = 0;
	if ((hours_20s == 0) && (hours > 9))
		hours_10s = 1;
	uint8_t hours_1s = hours - ((hours / 10) * 10);

	return ((hours_20s << 5) | (hours_10s << 4) | (hours_1s));
}

/*
 * Sets the given bit in the given register of the RTC.
 */
static void set_bit(uint8_t register_addr, uint8_t to_set)
{
	uint8_t old_byte = read_byte(register_addr);
	static uint8_t new_byte[1];
	new_byte[0] = (old_byte | (1 << to_set));

	i2c_write_byte_to_device(MODULE_ADDRESS, register_addr, new_byte, 1);
}

/*
 * Clears the given bit in the given register of the RTC.
 */
static void clear_bit(uint8_t register_addr, uint8_t to_clear)
{
	uint8_t old_byte = read_byte(register_addr);
	static uint8_t new_byte[1];
	new_byte[0] = (old_byte & ~(1 << to_clear));

	i2c_write_byte_to_device(MODULE_ADDRESS, register_addr, new_byte, 1);
}

static uint8_t read_byte(uint8_t byte_address)
{
	static uint8_t byte[1];
	i2c_read_bytes_from_device(MODULE_ADDRESS, byte_address, byte, 1);

	return byte[0];
}
