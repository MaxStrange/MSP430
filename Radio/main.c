#include <msp430.h> 
#include "my_led.h"
#include "my_button.h"
#include "radio.h"

static inline void handle_rx_data(void);

static volatile int which_led_is_on = 0;

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /*
     * Initialize modules
     */
    led_init();
    button_init();
    radio_init();


    /*
     * Initialize state
     */
    led_off();
    _enable_interrupts();//globally enable interrupts


    /*
     * Loop
     */
    while (1)
    {
    	;//everything is taken care of using interrupts
    }

	return 0;
}

#pragma vector=PORT4_VECTOR
__interrupt void PORT4_ISR(void)
{
	//debounce
	__delay_cycles(1500);
	if (!BUTTON_PRESSED)
		return;

	radio_write(which_led_is_on);

	P4IFG = 0x00;//Make sure you clear the interrupt flag before you return
}

static inline void handle_rx_data(void)
{
	char data = radio_read();

	which_led_is_on++;

	if (which_led_is_on > 8)
		which_led_is_on = 0;

	led_on_i(which_led_is_on);
}

#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
	switch (__even_in_range(UCB0IV, 18))
	{
	case 0x00://Vector 0: No interrupt... not sure what that means
		break;

	case 0x02://UCRXIFG -- received some data in the rx buf
		handle_rx_data();
		UCB0IFG &= ~UCRXIFG;
		break;

	case 0x04://UCTXIFG -- sent some data from the tx buf
		if (UCB0IFG & UCTXIFG)
		{
			UCB0IE &= ~UCTXIE;
			UCB0IFG &= ~UCTXIFG;
		}
		break;

	case 0x06://UCSTTIFG -- start byte received
		break;

	case 0x08://UCTXCPTIFG -- sent all the data from the tx buf including a stop bit
		break;

	default:
		break;
	}
}
