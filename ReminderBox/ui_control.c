#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "menu.h"

#include "ui_control.h"


volatile static menu_system_t *current_menu;
volatile static bool *sleep;


void ui_control_init(volatile bool *sleep_ptr, volatile menu_system_t *menu)
{
	//set buttons and quad encoder to input
	P1DIR &= ~(CONFIRM_BUTTON | REJECT_BUTTON);
	P2DIR &= ~(QEN_BLUE | QEN_GREEN);

	//set the pullup/pulldown resistors for the buttons
	P1REN |= CONFIRM_BUTTON | REJECT_BUTTON;
	//TODO : Figure out whether the quad encoder needs resistors

	//set the resistors as pulldowns
	P1OUT &= ~(CONFIRM_BUTTON | REJECT_BUTTON);

	sleep = sleep_ptr;
	current_menu = menu;
}

void ui_control_start(void)
{
    /*
     * Initialize interrupts for this module
     * These SHOULD NOT be removed upon sleep, since then they wouldn't be able to wake the box back up.
     */
    P1IES &= ~(CONFIRM_BUTTON | REJECT_BUTTON);
    P1IE |= CONFIRM_BUTTON | REJECT_BUTTON;

    //TODO initialize the quad encoder interrupts too

    //TODO initialize the countdown timer with interrupt that will cause sleep to become true when it fires
}

/*
 * Debounces a button. Returns true if button fire is a real one. False if it was
 * just a bounce and should be ignored.
 */
inline static bool debounce(bool red)
{
	if (red)
		__delay_cycles(200000);
	else
		__delay_cycles(1500);

	bool button_pressed = (P1IN & CONFIRM_BUTTON) | (P1IN & REJECT_BUTTON);
	if (!button_pressed)
	{
		P1IFG &= ~(CONFIRM_BUTTON | REJECT_BUTTON);
		return false;
	}

	return true;
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	//Which peripheral fired the interrupt?

	if (P1IN & CONFIRM_BUTTON)
	{
		if (debounce(false))
			current_menu->confirm(current_menu, &current_menu->current_choice);
	}
	else if (P1IN & REJECT_BUTTON)
	{
		if (debounce(true))
			current_menu->reject(current_menu, current_menu->current_choice);
	}
	else
	{
		//TODO Quad encoder interrupt
	}


	P1IFG &= ~(CONFIRM_BUTTON | REJECT_BUTTON);//Make sure you clear the interrupt flag before you return
}

