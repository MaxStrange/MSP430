/*
 * This program illustrates how to use interrupts to service a push button request.
 */

#include <msp430.h> 

#define BUTTON BIT0
#define BUTTON_PRESSED ((P4IN & BUTTON) == 0x00)

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    //enable pullup resistor on the button pin
    P4REN |= BUTTON;
    P4OUT |= BUTTON;

    init_led();
    led_off();

    P4IE |= BUTTON;//enable interrupt on button's pin
    P4IES |= BUTTON;//edge select from high to low (since signal is low when button is pressed)
    P4IFG = 0x00;//make sure the interrupt flag is clear

    _enable_interrupts();//globally enable interrupts

    while (1)
    {
    	;//everything is taken care of using interrupts
    }

	return 0;
}

#pragma vector=PORT4_VECTOR
__interrupt void PORT4_ISR(void)
{
	//do whatever you want to do in the interrupt

	P4IFG = 0x00;//Make sure you clear the interrupt flag before you return
}
