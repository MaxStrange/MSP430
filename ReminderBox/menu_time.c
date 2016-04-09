#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "menu.h"

#include "menu_time.h"


static void (*scroll_top_forward_fp)(volatile menu_choice_t *, volatile menu_choice_t *);
static void (*scroll_top_backward_fp)(volatile menu_choice_t *, volatile menu_choice_t *);
static void (*confirm_top_fp)(volatile menu_system_t *, volatile menu_choice_t *);
static void (*reject_top_fp)(volatile menu_system_t *, volatile menu_choice_t *);



static void scroll_choice(volatile menu_system_t *menu, volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice);
static void confirm_noop(volatile menu_system_t *menu, volatile menu_choice_t *current_item);
static void reject_choice_go_back_to_top_level(volatile menu_system_t *menu, volatile menu_choice_t *current_item);


volatile menu_system_t time_menu =
{
		.current_choice = (menu_choice_t)CHOICE_DISPLAY_TIME,
		.scroll_menu_forward = (scroll_menu_fp)&scroll_choice,
		.scroll_menu_backward = (scroll_menu_fp)&scroll_choice,
		.confirm = (confirm_fp)&confirm_noop,
		.reject = (reject_fp)&reject_choice_go_back_to_top_level
};



void menu_time_init(void (*scroll_forward_fp)(volatile menu_choice_t*, volatile menu_choice_t *), void (*scroll_backward_fp)(volatile menu_choice_t *, volatile menu_choice_t *),
		void (*confirm_fp)(volatile menu_system_t *, volatile menu_choice_t *), void (*reject_fp)(volatile menu_system_t *, volatile menu_choice_t *))
{
	scroll_top_forward_fp = scroll_forward_fp;
	scroll_top_backward_fp = scroll_backward_fp;
	confirm_top_fp = confirm_fp;
	reject_top_fp = reject_fp;
}

inline static void scroll_choice(volatile menu_system_t *menu, volatile menu_choice_t *current_item, volatile menu_choice_t *current_sub_choice)
{
	//Do nothing
}

inline static void confirm_noop(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	//Do nothing
}

inline static void reject_choice_go_back_to_top_level(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	//*menu = top_menu;//This line doesn't work.

	//Currently I can do this manual work around - fill in each of menu's items manually. TODO Find out why I can't do it in one line.

	menu->scroll_menu_forward = scroll_top_forward_fp;
	menu->scroll_menu_backward = scroll_top_backward_fp;
	menu->confirm = confirm_top_fp;
	menu->reject = reject_top_fp;

	*current_item = (menu_choice_t)DISPLAY_TIME;
}

