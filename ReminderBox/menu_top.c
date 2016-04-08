#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "menu.h"
#include "menu_time.h"
#include "menu_pay_bill.h"
#include "menu_check_due.h"
#include "menu_enter_bill.h"
#include "menu_set_time.h"

#include "menu_top.h"


volatile menu_system_t top_menu =
{
		.current_choice = (menu_choice_t)DISPLAY_TIME,
		.scroll_menu_forward = &scroll_top_menu_forward,
		.scroll_menu_backward = &scroll_top_menu_backward,
		.confirm = &confirm_top_menu,
		.reject = &reject_top_menu
};


inline void scroll_top_menu_forward(volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice)
{
	menu_choice_t m = *current_item;

	switch (m)
	{
	case DISPLAY_TIME:
		*current_item = CHECK_DUE_DATES;
		break;
	case CHECK_DUE_DATES:
		*current_item = PAY_BILL;
		break;
	case PAY_BILL:
		*current_item = ENTER_NEW_DATE;
		break;
	case ENTER_NEW_DATE:
		*current_item = SET_TIME;
		break;
	case SET_TIME:
		*current_item = DISPLAY_TIME;
		break;
	}
}

inline void scroll_top_menu_backward(volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice)
{
	menu_choice_t m = *current_item;

	switch (m)
	{
	case DISPLAY_TIME:
		*current_item = SET_TIME;
		break;
	case CHECK_DUE_DATES:
		*current_item = DISPLAY_TIME;
		break;
	case PAY_BILL:
		*current_item = CHECK_DUE_DATES;
		break;
	case ENTER_NEW_DATE:
		*current_item = PAY_BILL;
		break;
	case SET_TIME:
		*current_item = ENTER_NEW_DATE;
		break;
	}
}

inline void confirm_top_menu(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	menu_choice_t m = *current_item;

	switch (m)
	{
	case DISPLAY_TIME:
		*menu = time_menu;
		*current_item = (menu_choice_t)CHOICE_DISPLAY_TIME;
		break;
	case CHECK_DUE_DATES:
//		menu->confirm = &confirm_noop(menu, current_item);
//		menu->reject = &reject_choice_go_back_to_top_level(menu, current_item);
//		menu->scroll_menu_forward = &scroll_choice(current_item);
//		menu->scroll_menu_backward = &scroll_choice(current_item);
		*menu = check_due_menu;
		*current_item = (menu_choice_t)CHOICE_DUE_MENU;
		break;
	case PAY_BILL:
//		menu->confirm = &confirm_noop(menu, current_item);
//		menu->reject = &reject_choice_go_back_to_top_level(menu, current_item);
//		menu->scroll_menu_forward = &scroll_choice(current_item);
//		menu->scroll_menu_backward = &scroll_choice(current_item);
		*menu = pay_bill_menu;
		*current_item = (menu_choice_t)CHOICE_PAY_MENU;
		break;
	case ENTER_NEW_DATE:
//		menu->confirm = &confirm_noop(menu, current_item);
//		menu->reject = &reject_choice_go_back_to_top_level(menu, current_item);
//		menu->scroll_menu_forward = &scroll_choice(current_item);
//		menu->scroll_menu_backward = &scroll_choice(current_item);
		*menu = enter_bill_menu;
		*current_item = (menu_choice_t)CHOICE_ENTER_BILL;
		break;
	case SET_TIME:
//		menu->confirm = &confirm_noop(menu, current_item);
//		menu->reject = &reject_choice_go_back_to_top_level(menu, current_item);
//		menu->scroll_menu_forward = &scroll_choice(current_item);
//		menu->scroll_menu_backward = &scroll_choice(current_item);
		*menu = set_time_menu;
		*current_item = (menu_choice_t)CHOICE_SET_TIME;
		break;
	}
}

inline void reject_top_menu(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	//Rejecting on the top level menu doesn't do anything.
}




