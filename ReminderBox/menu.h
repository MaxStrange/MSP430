#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>
#include <stdbool.h>

#include "bill.h"




typedef int menu_choice_t;

typedef void (*confirm_fp)(volatile struct menu_system_t *, volatile menu_choice_t *);
typedef void (*reject_fp)(volatile struct menu_system_t *, volatile menu_choice_t *);
typedef void (*scroll_menu_fp)(volatile struct menu_system_t *, volatile menu_choice_t *, volatile menu_choice_t *);


typedef struct menu_system_t
{
	menu_choice_t current_choice;
	menu_choice_t current_sub_menu_choice;
	bill_t selected_bill;

	confirm_fp confirm;
	reject_fp reject;
	scroll_menu_fp scroll_menu_forward;
	scroll_menu_fp scroll_menu_backward;
} menu_system_t;





#define TOP_MENU_SIZE 5
typedef enum
{
	DISPLAY_TIME = 0,
	CHECK_DUE_DATES = 1,
	PAY_BILL = 2,
	ENTER_NEW_DATE = 3,
	SET_TIME = 4
} top_menu_e;

typedef enum
{
	CHOICE_DISPLAY_TIME = 5
} choice_time_e;

typedef enum
{
	CHOICE_DUE_MENU = 6
} choice_due_e;

typedef enum
{
	CHOICE_PAY_MENU = 7
} choice_pay_e;

typedef enum
{
	CHOICE_ENTER_BILL = 8
} choice_enter_bill_e;

typedef enum
{
	CHOICE_SET_TIME = 9
} choice_set_time_e;


typedef enum
{
//DAY OF THE MONTH - 1 - 31 reserved
//PEOPLE
	SUB_CHOICE_MAX = 32,
	SUB_CHOICE_MISH = 33,
//COMPANIES
	CHASE_FREEDOM = 34,
	CHASE_SAPPHIRE = 35,
	CAPITAL_ONE = 36,
	DISCOVER = 37,
	AMEX = 38,
	CITI = 39,
	ELECTRICITY = 40,
	WATER = 41,
	COMCAST = 42,
	MEDICAL = 43,
	MISC = 44,
//Months
	JANUARY = 45,
	FEBRUARY = 46,
	MARCH = 47,
	APRIL = 48,
	MAY = 49,
	JUNE = 50,
	JULY = 51,
	AUGUST = 52,
	SEPTEMBER = 53,
	OCTOBER = 54,
	NOVEMBER = 55,
	DECEMBER = 56,
//Command
	CONFIRM_ENTER_BILL = 57
} sub_choice_enter_bill_e;

typedef enum
{
	WAIT_E = 255
} wait_e;

#endif /* MENU_H_ */
