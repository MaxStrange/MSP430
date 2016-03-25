#ifndef REAL_TIME_CLOCK_H_
#define REAL_TIME_CLOCK_H_

#include <stdint.h>

#define MODULE_ADDRESS 0xD0//7 bit address: 1101 000

#define ADDR_TIME_SECONDS	0x00
#define ADDR_TIME_MINUTES	0x01
#define ADDR_TIME_HOURS		0x02
#define ADDR_TIME_DAY		0x03
#define ADDR_TIME_DATE		0x04
#define ADDR_TIME_MONTH		0x05
#define ADDR_TIME_YEAR		0x06
#define ADDR_ALARM_SECONDS	0x07
#define ADDR_ALARM_MINUTES	0x08
#define ADDR_ALARM_HOURS	0x09
#define ADDR_ALARM_DATE		0x0A
#define ADDR_ALARM2_MINUTES	0x0B
#define ADDR_ALARM2_HOURS	0x0C
#define ADDR_ALARM2_DATE	0x0D



typedef enum
{
	SUNDAY = 1,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY
} e_day_of_week_t;


void rtc_set_time(uint8_t minutes, uint8_t hours_24, e_day_of_week_t day, uint8_t date, uint8_t month, uint8_t year_since_2000);
void rtc_get_time(uint8_t *data_array_len_7);

#endif /* REAL_TIME_CLOCK_H_ */
