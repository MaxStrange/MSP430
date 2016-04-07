#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "ui_control.h"
#include "ui_view.h"
#include "menu.h"

#include "ui_model.h"


/*
 * State
 */

volatile static bool sleep = false;


/*
 * Static functions
 */

static void scroll_top_menu_forward(volatile menu_choice_t *current_item);
static void scroll_top_menu_backward(volatile menu_choice_t *current_item);
static void confirm_top_menu(volatile menu_system_t *menu, volatile menu_choice_t *current_item);
static void reject_top_menu(volatile menu_system_t *menu, volatile menu_choice_t *current_item);
static void scroll_choice(volatile menu_choice_t *current_item);
static void confirm_noop(volatile menu_system_t *menu, volatile menu_choice_t *current_item);
static void reject_choice_go_back_to_top_level(volatile menu_system_t *menu, volatile menu_choice_t *current_item);



volatile static menu_system_t top_menu =
{
	.current_choice = (menu_choice_t)DISPLAY_TIME,
	.scroll_menu_forward = &scroll_top_menu_forward,
	.scroll_menu_backward = &scroll_top_menu_backward,
	.confirm = &confirm_top_menu,
	.reject = &reject_top_menu
};

volatile static menu_system_t time_menu =
{
		.current_choice = (menu_choice_t)CHOICE_DISPLAY_TIME,
		.scroll_menu_forward = &scroll_choice,
		.scroll_menu_backward = &scroll_choice,
		.confirm = &confirm_noop,
		.reject = &reject_choice_go_back_to_top_level
};



void ui_model_init(void)
{
	ui_control_init(&sleep, &top_menu);
	ui_view_init(&sleep, &top_menu);
}

void ui_model_wake(void)
{
	sleep = false;
	ui_control_start();
	ui_view_display();
}

inline static void scroll_top_menu_forward(volatile menu_choice_t *current_item)
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

inline static void scroll_top_menu_backward(volatile menu_choice_t *current_item)
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

inline static void confirm_top_menu(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	menu_choice_t m = *current_item;

	switch (m)
	{
	case DISPLAY_TIME:
		*menu = time_menu;
		break;
	case CHECK_DUE_DATES:
		break;
	case PAY_BILL:
		break;
	case ENTER_NEW_DATE:
		break;
	case SET_TIME:
		break;
	}
}

inline static void reject_top_menu(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	//Rejecting on the top level menu doesn't do anything.
}

inline static void scroll_choice(volatile menu_choice_t *current_item)
{
	//Do nothing
}

inline static void confirm_noop(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	//Do nothing
}

inline static void reject_choice_go_back_to_top_level(volatile menu_system_t *menu, volatile menu_choice_t *current_item)
{
	*menu = top_menu;
	*current_item = (menu_choice_t)DISPLAY_TIME;
}




