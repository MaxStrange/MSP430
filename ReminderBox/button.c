#include <msp430.h>

#include "clock.h"

#include "button.h"

void button_init(void)
{
	//set pins to be GPIO
	P2SEL &= ~(CONFIRM | REJECT);
	P2SEL2 &= ~(CONFIRM | REJECT);

    //enable pullup resistor on the button pins
    P2REN |= CONFIRM | REJECT;		//enable resistor - sets the pins as inputs as well
    P2OUT |= CONFIRM | REJECT;		//set resistors to be pull-ups, rather than downs

    //interrupts
    P2IFG = 0x00;					//make sure the interrupt flag is clear
    P2IES |= CONFIRM | REJECT;		//edge select from high to low (since signal is low when button is pressed)
    P2IE |= CONFIRM | REJECT;		//enable interrupt on button's pin
}

