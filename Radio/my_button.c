#include <msp430.h>
#include "my_button.h"


void button_init(void)
{
    //enable pullup resistor on the button pin
    P4REN |= BUTTON;
    P4OUT |= BUTTON;

    //interrupts
    P4IE |= BUTTON;//enable interrupt on button's pin
    P4IES |= BUTTON;//edge select from high to low (since signal is low when button is pressed)
    P4IFG = 0x00;//make sure the interrupt flag is clear
}
