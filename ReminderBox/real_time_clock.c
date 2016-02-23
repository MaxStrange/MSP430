#include <msp430.h>
#include <stdint.h>

#include "I2C.h"

#include "real_time_clock.h"


void rtc_set_time(uint8_t minutes, uint8_t hours_24, e_day_of_week_t day, uint8_t date, uint8_t month, uint8_t year_since_2000)
{
	uint8_t minute_10s = minutes / 10;
	uint8_t minute_1s = minutes - (minute_10s * 10);

	uint8_t hours_20s = (hours_24 > 19) ? 1 : 0;
	uint8_t hours_10s = 0;
	if ((hours_20s == 0) && (hours_24 > 9))
		hours_10s = 1;
	uint8_t hours_1s = hours_24 - ((hours_24 / 10) * 10);

	uint8_t date_10s = date / 10;
	uint8_t date_1s = date - (date_10s * 10);

	uint8_t month_10s = month / 10;
	uint8_t month_1s = month - (month_10s * 10);

	uint8_t year_10s = year_since_2000 / 10;
	uint8_t year_1s = year_since_2000 - (year_10s * 10);


	static uint8_t data_array[7];
	data_array[0] = 0;//seconds
	data_array[1] = (minute_10s << 4) | (minute_1s);//minutes;
	data_array[2] = (hours_20s << 5) | (hours_10s << 4) | (hours_1s);//hours_24;
	data_array[3] = (uint8_t)day;
	data_array[4] = (date_10s << 4) | (date_1s);//date;
	data_array[5] = (month_10s << 4) | (month_1s);//month;
	data_array[6] = (year_10s << 4) | (year_1s);//year_since_2000;

	i2c_write_byte_to_device(MODULE_ADDRESS, ADDR_TIME_SECONDS, data_array, 7);
}
