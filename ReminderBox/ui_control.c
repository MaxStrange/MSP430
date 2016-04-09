#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#include "menu.h"

#include "ui_control.h"



typedef enum
{
	LBLG,		//Low blue, low green... etc.
	HBLG,
	HBHG,
	LBHG
} encoder_state_e;

volatile static menu_system_t *current_menu;
volatile static bool *sleep;


void ui_control_init(volatile bool *sleep_ptr, volatile menu_system_t *menu)
{
	//Configure the quad pins as GPIO
	P2SEL &= ~(QEN_BLUE | QEN_GREEN);
	P2SEL2 &= ~(QEN_BLUE | QEN_GREEN);

	//set buttons and quad encoder to input
	P1DIR &= ~(CONFIRM_BUTTON | REJECT_BUTTON);
	P2DIR &= ~(QEN_BLUE | QEN_GREEN);

	//set the pullup/pulldown resistors for the buttons and quad encoder
	P1REN |= CONFIRM_BUTTON | REJECT_BUTTON;
	P2REN |= QEN_BLUE | QEN_GREEN;

	//set the resistors as pulldowns
	P1OUT &= ~(CONFIRM_BUTTON | REJECT_BUTTON);
	P2OUT &= ~(QEN_BLUE | QEN_GREEN);

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

//    P2IES &= ~(QEN_GREEN | QEN_BLUE);//fire on rising edge
//    P2IE |= QEN_GREEN | QEN_BLUE;
    P2IES &= ~QEN_GREEN;
    P2IE |= QEN_GREEN;

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
	//Which button fired the interrupt?

	if (P1IN & CONFIRM_BUTTON)
	{
		if (debounce(false))
			current_menu->confirm(current_menu, &current_menu->current_choice);
	}
	else if (P1IN & REJECT_BUTTON)
	{
		if (debounce(true))
			current_menu->reject(current_menu, &current_menu->current_choice);
	}

	P1IFG &= ~(CONFIRM_BUTTON | REJECT_BUTTON);//Make sure you clear the interrupt flag before you return
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
	//debounce the switch
	__delay_cycles(5000);
	if (! ((P2IN & QEN_GREEN) || (P2IN & QEN_BLUE)))
	{
		P2IFG &= ~(QEN_GREEN | QEN_BLUE);//Make sure you clear the interrupt flag before you return
		return;
	}


	volatile static unsigned int number_of_turns = 0;
	volatile static bool clockwise = false;

	if (P2IN & QEN_GREEN)
	{
		if (P2IN & QEN_BLUE)
		{
			clockwise = false;
		}
		else
		{
			clockwise = true;
		}
	}

//	if (P2IN & QEN_BLUE)
//	{
//	}


	number_of_turns++;

	if (number_of_turns > 1)
	{
		number_of_turns = 0;
		if (clockwise)
			current_menu->scroll_menu_forward(current_menu, &(current_menu->current_choice), &(current_menu->current_sub_menu_choice));
		else
			current_menu->scroll_menu_backward(current_menu, &(current_menu->current_choice), &(current_menu->current_sub_menu_choice));
	}



	P2IFG &= ~(QEN_GREEN | QEN_BLUE);//Make sure you clear the interrupt flag before you return
}

