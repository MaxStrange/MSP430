/*
 * UI_MODEL module.
 *
 * This module is the model for the MVC pattern of the user interface. It is also the only one of the three MVC modules
 * that is exposed to the outside. The user_interface module includes this one and interacts with it.
 *
 * The model module attempts to contain all of the state required by the user interface.
 * It contains all of the different menu systems/subsystems and associated functions, and it packages them
 * into types found in the menu.h file.
 *
 * The model module initializes and starts the view and control. When it does this, it passes in to those modules
 * pointers to the important state in this module. That way, the view can read the current state and display appropriate
 * information via a while loop, while the control manipulates the state via interrupts.
 */


#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "ui_control.h"
#include "ui_view.h"
#include "menu.h"
#include "menu_top.h"
#include "menu_time.h"
#include "menu_check_due.h"
#include "menu_enter_bill.h"
#include "menu_pay_bill.h"
#include "menu_set_time.h"

#include "ui_model.h"


volatile static bool sleep = false;


void ui_model_init(void)
{
	menu_time_init(&scroll_top_menu_forward, &scroll_top_menu_backward, &confirm_top_menu, &reject_top_menu);
	menu_check_due_init(&scroll_top_menu_forward, &scroll_top_menu_backward, &confirm_top_menu, &reject_top_menu);
	menu_enter_bill_init(&scroll_top_menu_forward, &scroll_top_menu_backward, &confirm_top_menu, &reject_top_menu);
	menu_pay_bill_init(&scroll_top_menu_forward, &scroll_top_menu_backward, &confirm_top_menu, &reject_top_menu);
	menu_set_time_init(&scroll_top_menu_forward, &scroll_top_menu_backward, &confirm_top_menu, &reject_top_menu);


	ui_view_init(&sleep, &top_menu);
	ui_control_init(&sleep, &top_menu);
}

void ui_model_wake(void)
{
	sleep = false;
	ui_control_start();
	ui_view_display();
}


