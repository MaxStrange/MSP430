#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "menu.h"

#include "menu_check_due.h"

static void scroll_due_menu_forward(volatile menu_choice_t *current_item);
static void scroll_due_menu_backward(volatile menu_choice_t *current_item);
static void confirm_check_due(volatile menu_system_t *menu, volatile menu_choice_t *current_item);
static void reject_check_due(volatile menu_system_t *menu, volatile menu_choice_t *current_item);



static void (*scroll_top_forward_fp)(volatile menu_choice_t *);
static void (*scroll_top_backward_fp)(volatile menu_choice_t *);
static void (*confirm_top_fp)(volatile menu_system_t *, volatile menu_choice_t *);
static void (*reject_top_fp)(volatile menu_system_t *, volatile menu_choice_t *);

volatile menu_system_t check_due_menu =
{
		.current_choice = (menu_choice_t)CHOICE_DUE_MENU,
		.scroll_menu_forward = &scroll_due_menu_forward,
		.scroll_menu_backward = &scroll_due_menu_backward,
		.confirm = &confirm_check_due,
		.reject = &reject_check_due
};


void menu_check_due_init(void (*scroll_forward_fp)(volatile menu_choice_t*), void (*scroll_backward_fp)(volatile menu_choice_t *),
		void (*confirm_fp)(volatile menu_system_t *, volatile menu_choice_t *), void (*reject_fp)(volatile menu_system_t *, volatile menu_choice_t *))
{
	scroll_top_forward_fp = scroll_forward_fp;
	scroll_top_backward_fp = scroll_backward_fp;
	confirm_top_fp = confirm_fp;
	reject_top_fp = reject_fp;
}

inline void scroll_due_menu_forward(volatile menu_choice_t *current_item)
{
}

inline void scroll_due_menu_backward(volatile menu_choice_t *current_item)
{
}

inline void confirm_check_due(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
}

inline void reject_check_due(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	menu->confirm = confirm_top_fp;
	menu->reject = reject_top_fp;
	menu->scroll_menu_backward = scroll_top_backward_fp;
	menu->scroll_menu_forward = scroll_top_forward_fp;

	*current_item = (menu_choice_t)CHECK_DUE_DATES;
}

