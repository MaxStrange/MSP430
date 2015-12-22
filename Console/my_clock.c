#include <msp430.h>
#include "my_clock.h"

void clock_init(void)
{//REMEMBER: IF YOU CHANGE ANY OF THIS, MAKE SURE TO CHANGE THE APPROPRIATE CONSTANTS IN THE HEADER FILE

	init_pins();

	/*
	 * See p.80 of user guide.
	 *
	 * To manipulate the clock module, first write the password to the
	 * CSCTL0_H register (password is 0xA5).
	 */

	CSCTL0_H = 0xA5;

	CSCTL2 |= SELA_0 + SELS_3 + SELM_3;         // Set the various clock sources: ACLK = XT1; MCLK and SMCLK = DCO
	CSCTL3 |= DIVA_0 + DIVS_3 + DIVM_3;         // set all dividers - ACLK / 1; SMCLK and MCLK / 8 (so 1 MHz).
	CSCTL4 |= XT1DRIVE_0;					   // set the XT1 oscillator current - lowest possible
	CSCTL4 &= ~XT1OFF;						   // turns on the XT1 as long as XT2 is selected by port selection and not in bypass mode


    /*
     * Since the XT1 is an oscillator crystal, it takes a bit of time to settle down
     * to a stable cycle. Loop indefinitely, checking whether it faulted or not
     * until it didn't fault.
     *
     * But for some reason, this doesn't seem to work, and instead it just loops
     * forever.
     *
     * TODO : Fix the clock
     */
//	do
//	{
//		CSCTL5 &= ~XT1OFFG;						//This bit says whether or not there is a fault in the XT1 - so clear it
//		SFRIFG1 &= ~OFIFG;						//This bit says whether there is a fault interrupt - so clear it
//	}while (SFRIFG1 & OFIFG);                     // Test to make sure there is no fault

	CSCTL0_H = 0x01;						    //Lock the CSC module now that it is configured


    //That should do it for setting up the clock system control.
    //To summarize: ACLK is sourced from the DCO and has been divided down to 1 MHz.
    //MCLK and SMCLK are both sourced from the XT2 crystal and are at 32.768 kHz
}

static void init_pins(void)
{
	//Have to set up the pins that will take in the onboard XT1 - see p.91 of datasheet (not UG)
	PJSEL0 |= BIT4 + BIT5;
	PJDIR |= BIT4 + BIT5;//TODO : check if this line made any difference in the external crystal clock source
}



