#ifndef BILL_H_
#define BILL_H_

typedef enum person_e
{
	MAX,
	MISH
} person_e;

typedef enum company_e
{
	CHASE_FREEDOM,
	CHASE_SAPPHIRE,
	CAPITAL_ONE,
	DISCOVER,
	AMEX,
	CITI,
	ELECTRICITY,
	WATER,
	COMCAST,
	MEDICAL,
	MISC
} company_e;

typedef enum month_e
{
	JANUARY,
	FEBRUARY,
	MARCH,
	APRIL,
	MAY,
	JUNE,
	JULY,
	AUGUST,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER
} month_e;

typedef struct date_t
{
	month_e month,
	uint8_t day
} date_t;

typedef struct bill_t
{
	company_e company,
	person_e person,
	date_t date
} bill_t;



#endif /* BILL_H_ */
