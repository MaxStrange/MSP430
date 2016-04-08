#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "menu.h"

#include "menu_enter_bill.h"


static void scroll_enter_bill_forward(volatile menu_choice_t *current_item);
static void scroll_enter_bill_backward(volatile menu_choice_t *current_item);
static void confirm_enter_bill(volatile menu_system_t *menu, volatile menu_choice_t *current_item);
static void reject_enter_bill(volatile menu_system_t *menu, volatile menu_choice_t *current_item);


static void (*scroll_top_forward_fp)(volatile menu_choice_t *);
static void (*scroll_top_backward_fp)(volatile menu_choice_t *);
static void (*confirm_top_fp)(volatile menu_system_t *, volatile menu_choice_t *);
static void (*reject_top_fp)(volatile menu_system_t *, volatile menu_choice_t *);


volatile menu_system_t enter_bill_menu =
{
		.current_choice = (menu_choice_t)CHOICE_ENTER_BILL,
		.scroll_menu_forward = &scroll_enter_bill_forward,
		.scroll_menu_backward = &scroll_enter_bill_backward,
		.confirm = &confirm_enter_bill,
		.reject = &reject_enter_bill
};



void menu_enter_bill_init(void (*scroll_forward_fp)(volatile menu_choice_t*), void (*scroll_backward_fp)(volatile menu_choice_t *),
		void (*confirm_fp)(volatile menu_system_t *, volatile menu_choice_t *), void (*reject_fp)(volatile menu_system_t *, volatile menu_choice_t *))
{
	scroll_top_forward_fp = scroll_forward_fp;
	scroll_top_backward_fp = scroll_backward_fp;
	confirm_top_fp = confirm_fp;
	reject_top_fp = reject_fp;
}


inline static void scroll_enter_bill_forward(volatile menu_choice_t *current_item)
{
}

inline static void scroll_enter_bill_backward(volatile menu_choice_t *current_item)
{
}

inline static void confirm_enter_bill(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
}

inline static void reject_enter_bill(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	//TODO redo - right now it just resets to main menu
	menu->confirm = confirm_top_fp;
	menu->reject = reject_top_fp;
	menu->scroll_menu_backward = scroll_top_backward_fp;
	menu->scroll_menu_forward = scroll_top_forward_fp;

	*current_item = (menu_choice_t)ENTER_NEW_DATE;
}
