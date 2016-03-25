#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "I2C.h"

#include "real_time_clock.h"


void rtc_set_time(uint8_t minutes, uint8_t hours_24, e_day_of_week_t day, uint8_t date, uint8_t month, uint8_t year_since_2000)
{
	//TEST DATA: 45, 14, TUESDAY, 23, 2, 16

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
	data_array[0] = (uint8_t)0;//seconds												//0x00
	data_array[1] = (minute_10s << 4) | (minute_1s);//minutes;							//0x52
	data_array[2] = (hours_20s << 5) | (hours_10s << 4) | (hours_1s);//hours_24;		//0x11
	data_array[3] = (uint8_t)day;														//0x06
	data_array[4] = (date_10s << 4) | (date_1s);//date;									//0x25
	data_array[5] = (month_10s << 4) | (month_1s);//month;								//0x03
	data_array[6] = (year_10s << 4) | (year_1s);//year_since_2000;						//0x16

	i2c_write_byte_to_device(MODULE_ADDRESS, ADDR_TIME_SECONDS, data_array, 7);
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
