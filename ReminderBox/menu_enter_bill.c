#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "bill.h"
#include "menu.h"

#include "menu_enter_bill.h"


static void scroll_enter_bill_forward(volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice);
static void scroll_enter_bill_backward(volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice);
static void confirm_enter_bill(volatile menu_system_t *menu, volatile menu_choice_t *current_item);
static void reject_enter_bill(volatile menu_system_t *menu, volatile menu_choice_t *current_item);


static void (*scroll_top_forward_fp)(volatile menu_choice_t *, volatile menu_choice_t *);
static void (*scroll_top_backward_fp)(volatile menu_choice_t *, volatile menu_choice_t *);
static void (*confirm_top_fp)(volatile menu_system_t *, volatile menu_choice_t *);
static void (*reject_top_fp)(volatile menu_system_t *, volatile menu_choice_t *);

volatile menu_system_t enter_bill_menu =
{
		.current_choice = (menu_choice_t)CHOICE_ENTER_BILL,
		.current_sub_menu_choice = (menu_choice_t)SUB_CHOICE_MISH,
		.scroll_menu_forward = &scroll_enter_bill_forward,
		.scroll_menu_backward = &scroll_enter_bill_backward,
		.confirm = &confirm_enter_bill,
		.reject = &reject_enter_bill
};


volatile static bill_t current_bill =
{
		//default values are fine - this will get filled in each time the user selects the pertinant info
};



void menu_enter_bill_init(void (*scroll_forward_fp)(volatile menu_choice_t*, volatile menu_choice_t *), void (*scroll_backward_fp)(volatile menu_choice_t *, volatile menu_choice_t *),
		void (*confirm_fp)(volatile menu_system_t *, volatile menu_choice_t *), void (*reject_fp)(volatile menu_system_t *, volatile menu_choice_t *))
{
	scroll_top_forward_fp = scroll_forward_fp;
	scroll_top_backward_fp = scroll_backward_fp;
	confirm_top_fp = confirm_fp;
	reject_top_fp = reject_fp;
}


inline static void scroll_enter_bill_forward(volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice)
{
	switch (*current_sub_choice)
	{
	//PEOPLE
	case SUB_CHOICE_MAX:
		*current_sub_choice = SUB_CHOICE_MISH;
		break;
	case SUB_CHOICE_MISH:
		*current_sub_choice = SUB_CHOICE_MAX;
		break;
	//COMPANIES
	case CHASE_FREEDOM:
	case CHASE_SAPPHIRE:
	case CAPITAL_ONE:
	case DISCOVER:
	case AMEX:
	case CITI:
	case ELECTRICITY:
	case WATER:
	case COMCAST:
	case MEDICAL:
		*current_sub_choice += 1;
		break;
	case MISC:
		*current_sub_choice = CHASE_FREEDOM;
		break;
	//MONTHS
	case JANUARY:
	case FEBRUARY:
	case MARCH:
	case APRIL:
	case MAY:
	case JUNE:
	case JULY:
	case AUGUST:
	case SEPTEMBER:
	case OCTOBER:
	case NOVEMBER:
		*current_sub_choice += 1;
		break;
	case DECEMBER:
		*current_sub_choice = JANUARY;
		break;
	case CONFIRM_ENTER_BILL:
		//Do nothing on confirm screen
		break;
	case WAIT_E:
		//Do nothing while waiting
		break;
	default:
		//We are in date
		*current_sub_choice = (*current_sub_choice == 31) ? (1) : (*current_sub_choice + 1);
		break;
	}
}

inline static void scroll_enter_bill_backward(volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice)
{
	switch (*current_sub_choice)
	{
	//PEOPLE
	case SUB_CHOICE_MAX:
		*current_sub_choice = SUB_CHOICE_MISH;
		break;
	case SUB_CHOICE_MISH:
		*current_sub_choice = SUB_CHOICE_MAX;
		break;
	//COMPANIES
	case CHASE_FREEDOM:
		*current_sub_choice = MISC;
		break;
	case CHASE_SAPPHIRE:
	case CAPITAL_ONE:
	case DISCOVER:
	case AMEX:
	case CITI:
	case ELECTRICITY:
	case WATER:
	case COMCAST:
	case MEDICAL:
	case MISC:
		*current_sub_choice -= 1;
		break;
	//MONTHS
	case JANUARY:
		*current_sub_choice = DECEMBER;
		break;
	case FEBRUARY:
	case MARCH:
	case APRIL:
	case MAY:
	case JUNE:
	case JULY:
	case AUGUST:
	case SEPTEMBER:
	case OCTOBER:
	case NOVEMBER:
	case DECEMBER:
		*current_sub_choice -= 1;
		break;
	case CONFIRM_ENTER_BILL:
		//Do nothing on confirm screen
		break;
	case WAIT_E:
		//Do nothing while waiting
		break;
	default:
		//We are in date
		*current_sub_choice = (*current_sub_choice == 1) ? (31) : (*current_sub_choice - 1);
		break;
	}
}

inline static void confirm_enter_bill(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	switch (menu->current_sub_menu_choice)
	{
	//People
	case SUB_CHOICE_MAX:
		current_bill.person = MAX_P;
		menu->current_sub_menu_choice = CAPITAL_ONE;
		break;
	case SUB_CHOICE_MISH:
		current_bill.person = MISH_P;
		menu->current_sub_menu_choice = CHASE_FREEDOM;
		break;
	//Companies
	case CHASE_FREEDOM:
		current_bill.company = CHASE_FREEDOM_C;
		menu->current_sub_menu_choice = JANUARY;
		break;
	case CHASE_SAPPHIRE:
		current_bill.company = CHASE_SAPPHIRE_C;
		menu->current_sub_menu_choice = JANUARY;
		break;
	case CAPITAL_ONE:
		current_bill.company = CAPITAL_ONE_C;
		menu->current_sub_menu_choice = JANUARY;
		break;
	case DISCOVER:
		current_bill.company = DISCOVER_C;
		menu->current_sub_menu_choice = JANUARY;
		break;
	case AMEX:
		current_bill.company = AMEX_C;
		menu->current_sub_menu_choice = JANUARY;
		break;
	case CITI:
		current_bill.company = CITI_C;
		menu->current_sub_menu_choice = JANUARY;
		break;
	case ELECTRICITY:
		current_bill.company = ELECTRICITY_C;
		menu->current_sub_menu_choice = JANUARY;
		break;
	case WATER:
		current_bill.company = WATER_C;
		menu->current_sub_menu_choice = JANUARY;
		break;
	case COMCAST:
		current_bill.company = COMCAST_C;
		menu->current_sub_menu_choice = JANUARY;
		break;
	case MEDICAL:
		current_bill.company = MEDICAL_C;
		menu->current_sub_menu_choice = JANUARY;
		break;
	case MISC:
		current_bill.company = MISC_C;
		menu->current_sub_menu_choice = JANUARY;
		break;
	//Months
	case JANUARY:
		current_bill.date.month = JANUARY_M;
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case FEBRUARY:
		current_bill.date.month = FEBRUARY_M;
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case MARCH:
		current_bill.date.month = MARCH_M;
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case APRIL:
		current_bill.date.month = APRIL_M;
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case MAY:
		current_bill.date.month = MAY_M;
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case JUNE:
		current_bill.date.month = JUNE_M;
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case JULY:
		current_bill.date.month = JULY_M;
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case AUGUST:
		current_bill.date.month = AUGUST_M;
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case SEPTEMBER:
		current_bill.date.month = SEPTEMBER_M;
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case OCTOBER:
		current_bill.date.month = OCTOBER_M;
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case NOVEMBER:
		current_bill.date.month = NOVEMBER_M;
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case DECEMBER:
		current_bill.date.month = DECEMBER_M;
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case CONFIRM_ENTER_BILL:
		//TODO save the bill to the memory
		menu->current_sub_menu_choice = WAIT_E;
		break;
	case WAIT_E:
		//Do nothing while waiting
		break;
	default:
		//The default case is for when the sub choice is 1-31, in which case, it means that is the date
		current_bill.date.day = menu->current_sub_menu_choice;
		menu->current_sub_menu_choice = CONFIRM_ENTER_BILL;
		break;
	}
}

inline static void reject_enter_bill(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	switch (menu->current_sub_menu_choice)
	{
	case SUB_CHOICE_MAX:
	case SUB_CHOICE_MISH:
		//Go back to top menu
		menu->confirm = confirm_top_fp;
		menu->reject = reject_top_fp;
		menu->scroll_menu_backward = scroll_top_backward_fp;
		menu->scroll_menu_forward = scroll_top_forward_fp;
		*current_item = (menu_choice_t)ENTER_NEW_DATE;
		break;
	case CHASE_FREEDOM:
	case CHASE_SAPPHIRE:
	case CAPITAL_ONE:
	case DISCOVER:
	case AMEX:
	case CITI:
	case ELECTRICITY:
	case WATER:
	case COMCAST:
	case MEDICAL:
	case MISC:
		//Go back to max/mish choice
		menu->current_sub_menu_choice = SUB_CHOICE_MAX;
		break;
	case JANUARY:
	case FEBRUARY:
	case MARCH:
	case APRIL:
	case MAY:
	case JUNE:
	case JULY:
	case AUGUST:
	case SEPTEMBER:
	case OCTOBER:
	case NOVEMBER:
	case DECEMBER:
		//Go back to company choice
		menu->current_sub_menu_choice = (current_bill.person == MISH_P) ? CHASE_FREEDOM : CAPITAL_ONE;
		break;
	case CONFIRM_ENTER_BILL:
		//Go back to date:
		menu->current_sub_menu_choice = (menu_choice_t)1;
		break;
	case WAIT_E:
		//Do nothing while waiting
		//TODO this line is for debugging
		menu->current_sub_menu_choice = CONFIRM_ENTER_BILL;
		break;
	default:
		//We are in date, go back to month
		menu->current_sub_menu_choice = JANUARY;
		break;
	}
}
