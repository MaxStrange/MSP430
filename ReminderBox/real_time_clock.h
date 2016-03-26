#ifndef REAL_TIME_CLOCK_H_
#define REAL_TIME_CLOCK_H_

#include <stdint.h>
#include <stdbool.h>

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
#define ADDR_CONTROL		0x0E

#define CONTROL_BITS_ALARM1	0
#define CONTROL_BITS_ALARM2 1
#define CONTROL_BITS_INTCN	2
#define CONTROL_BITS_RS1	3
#define CONTROL_BITS_RS2	4
#define CONTROL_BITS_CONV	5
#define CONTROL_BITS_BBSQW	6
#define CONTROL_BITS_EOSC	7



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


void rtc_init(void);
void rtc_set_time(uint8_t minutes, uint8_t hours_24, e_day_of_week_t day, uint8_t date, uint8_t month, uint8_t year_since_2000);
void rtc_get_time(uint8_t *data_array_len_7);
void rtc_set_alarm1(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t date);
void rtc_set_alarm2(uint8_t minutes, uint8_t hours, uint8_t date);

bool rtc_is_alarm1_set();
bool rtc_is_alarm2_set();

#endif /* REAL_TIME_CLOCK_H_ */
