#ifndef BILL_H_
#define BILL_H_


typedef enum person_e
{
	MAX_P,
	MISH_P
} person_e;

typedef enum company_e
{
	CHASE_FREEDOM_C,
	CHASE_SAPPHIRE_C,
	CAPITAL_ONE_C,
	DISCOVER_C,
	AMEX_C,
	CITI_C,
	ELECTRICITY_C,
	WATER_C,
	COMCAST_C,
	MEDICAL_C,
	MISC_C
} company_e;

typedef enum month_e
{
	JANUARY_M,
	FEBRUARY_M,
	MARCH_M,
	APRIL_M,
	MAY_M,
	JUNE_M,
	JULY_M,
	AUGUST_M,
	SEPTEMBER_M,
	OCTOBER_M,
	NOVEMBER_M,
	DECEMBER_M
} month_e;

typedef struct date_t
{
	month_e month;
	uint8_t day;
} date_t;

typedef struct bill_t
{
	company_e company;
	person_e person;
	date_t date;
} bill_t;



void bill_write_to_memory(bill_t *bill);
void bill_read_next_from_memory(bill_t *bill);
void bill_read_last_from_memory(bill_t *bill);

#endif /* BILL_H_ */
