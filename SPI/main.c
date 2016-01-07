/*
 * This program illustrates how to use interrupts to service a push button request.
 */

#include <msp430.h> 
#include "my_led.h"
#include "my_button.h"
#include "spi.h"

static inline void handle_rx_data(void);

static volatile int leds_are_on = 0;

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
    spi_init();


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

	UCA0TXBUF = leds_are_on;
	UCA0IE |= UCTXIE;

	P4IFG = 0x00;//Make sure you clear the interrupt flag before you return
}

static inline void handle_rx_data(void)
{
	int data = UCA0RXBUF;

//	if (data)
//		led_on();
//	else
//		led_off();

//	leds_are_on = !leds_are_on;
	leds_are_on++;

	if (leds_are_on > 8)
		leds_are_on = 0;

	led_on_i(leds_are_on);
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
	switch (__even_in_range(UCA0IV, 18))
	{
	case 0x00://Vector 0: No interrupt... not sure what that means
		break;

	case 0x02://UCRXIFG -- received some data in the rx buf
		handle_rx_data();
		UCA0IFG &= ~UCRXIFG;
		break;

	case 0x04://UCTXIFG -- sent some data from the tx buf
		if (UCA0IFG & UCTXIFG)
		{
			UCA0IE &= ~UCTXIE;
			UCA0IFG &= ~UCTXIFG;
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
