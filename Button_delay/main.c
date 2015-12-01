/*
 * An application to test a new delay function - one that will
 * look and work more like the Arduino's!
 *
 * Push the button and the LEDs will wait half a second and then
 * toggle their on/off state.
 */

#include <msp430.h> 
#include "my_led.h"

#define BUTTON BIT0
#define BUTTON_PRESSED ((P4IN & BUTTON) == 0x00)

void my_delay_ms(unsigned long int ms);

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

    while (1)
    {
    	if (BUTTON_PRESSED)
    	{
    		my_delay_ms(4000);
    		led_blink();
    	}
    }

	return 0;
}

/*
 * Seems to be quite approximate, possibly because the DCO is not a very accurate clock source?
 */
void my_delay_ms(unsigned long int ms)
{
	//Should check the clock source, then check its frequency and divide it down to whatever it is
	//by checking the dividers on the MCLCK, but for now let's just get this quick and dirty one working

	float mhz;

	//DCOFSEL is where you find the clock frequency of the DCO. It is bits 1 through 3 on the CSCTL1 register
	unsigned long int dcofsel = CSCTL1 & 0x0E;


	switch (dcofsel)
	{
	case 0x00:
		mhz = 1.0;
		break;
	case 0x01:
		mhz = 2.67;
		break;
	case 0x02:
		mhz = 3.33;
		break;
	case 0x03:
		mhz = 4.0;
		break;
	case 0x04:
		mhz = 5.33;
		break;
	case 0x05:
		mhz = 6.67;
		break;
	case 0x06:
		mhz = 8.0;//default value on startup
		break;
	case 0x07:
		mhz = 8.0;//This is not supposed to happen
		break;
	}


	//the mainclock is divided down by 8 by default, so:
	mhz /= 8;

	//mulitply by 1000 and truncate to get cycles / ms -- --- -- - - --- except not... multiply by 100 to get the real value.
	//Seem to be missing a factor of 10 somewhere but I don't know why. This works though... so...
	unsigned long int cycles_per_ms = (unsigned long int)(mhz * 100);

	//now delay (cycles_per_ms * ms) cycles to delay the appropriate number of ms
	volatile unsigned long int cycles = ms * cycles_per_ms;

	for (cycles; cycles > 0; cycles --);
}
