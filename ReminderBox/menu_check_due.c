#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "menu.h"
#include "bill.h"

#include "menu_check_due.h"

static void scroll_due_menu_forward(volatile menu_system_t *menu, volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice);
static void scroll_due_menu_backward(volatile menu_system_t *menu, volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice);
static void confirm_check_due(volatile menu_system_t *menu, volatile menu_choice_t *current_item);
static void reject_check_due(volatile menu_system_t *menu, volatile menu_choice_t *current_item);


static void (*scroll_top_forward_fp)(volatile menu_choice_t *, volatile menu_choice_t *);
static void (*scroll_top_backward_fp)(volatile menu_choice_t *, volatile menu_choice_t *);
static void (*confirm_top_fp)(volatile menu_system_t *, volatile menu_choice_t *);
static void (*reject_top_fp)(volatile menu_system_t *, volatile menu_choice_t *);


volatile menu_system_t check_due_menu =
{
		.current_choice = (menu_choice_t)CHOICE_DUE_MENU,
		.scroll_menu_forward = (scroll_menu_fp)&scroll_due_menu_forward,
		.scroll_menu_backward = (scroll_menu_fp)&scroll_due_menu_backward,
		.confirm = (confirm_fp)&confirm_check_due,
		.reject = (reject_fp)&reject_check_due,
		.selected_bill = { CAPITAL_ONE_C, MAX_P, { MAY_M, 0 } }
};


void menu_check_due_init(void (*scroll_forward_fp)(volatile menu_choice_t*, volatile menu_choice_t *), void (*scroll_backward_fp)(volatile menu_choice_t *, volatile menu_choice_t *),
		void (*confirm_fp)(volatile menu_system_t *, volatile menu_choice_t *), void (*reject_fp)(volatile menu_system_t *, volatile menu_choice_t *))
{
	scroll_top_forward_fp = scroll_forward_fp;
	scroll_top_backward_fp = scroll_backward_fp;
	confirm_top_fp = confirm_fp;
	reject_top_fp = reject_fp;
}

inline void scroll_due_menu_forward(volatile menu_system_t *menu, volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice)
{
	//Get the next bill from memory
	bill_t next_bill;
	bill_read_next_from_memory(&next_bill);

	if (next_bill.date.day == 65535)
		next_bill.date.day == 0;		//The view module sees a 0 in the day slot as meaning the data is no good

	menu->selected_bill = next_bill;

//	bill_t b = {
//			.company = CAPITAL_ONE_C,
//			.date = { JANUARY_M, 23 },
//			.person = MAX_P
//	};
//	menu->selected_bill = b;
}

inline void scroll_due_menu_backward(volatile menu_system_t *menu, volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice)
{
	//Get the last bill from memory
	bill_t last_bill;
	bill_read_last_from_memory(&last_bill);

	if (last_bill.date.day == 65535)
		last_bill.date.day = 0;

	menu->selected_bill = last_bill;

//	bill_t b = {
//			.company = CAPITAL_ONE_C,
//			.date = { JANUARY_M, 23 },
//			.person = MAX_P
//	};
//	menu->selected_bill = b;
}

inline void confirm_check_due(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	//Does nothing
}

inline void reject_check_due(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	menu->confirm = (confirm_fp)confirm_top_fp;
	menu->reject = (reject_fp)reject_top_fp;
	menu->scroll_menu_backward = (scroll_menu_fp)scroll_top_backward_fp;
	menu->scroll_menu_forward = (scroll_menu_fp)scroll_top_forward_fp;

	*current_item = (menu_choice_t)CHECK_DUE_DATES;
}

