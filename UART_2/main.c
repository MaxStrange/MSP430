#include <msp430.h> 

void init_clock(void);
void init_pins(void);
void init_UART(void);

int Data = 0;

/*
 * main.c
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
//
//    BCSCTL1 = CALBC1_1MHZ;
//    DCOCTL = CALDCO_1MHZ;
//
//    P1DIR |= BIT6;
//    P1OUT = 0x00;
//    P1SEL = BIT1 | BIT2;
//    P1SEL2 = BIT1 | BIT2;
//    P1IE |= 0x08;
//    P1IES |= 0x08;
//    P1IFG = 0x00;
//
//    UCA0CTL1 |= UCSSEL_2;
//    UCA0BR0 = 104;
//    UCA0BR1 = 0;
//    UCA0MCTL = UCBRS_1;
//    UCA0STAT |= UCLISTEN;
//    UCA0CTL1 &= ~UCSWRST;
//    IE2 |= UCA0RXIE | UCA0TXIE;
//
//    _enable_interrupts();
//
//    LPM4;

    init_clock();
    init_pins();
    init_UART();
}

void init_clock(void)
{
	/*
	 * See p.80 of user guide.
	 *
	 * To manipulate the clock module, first write the password to the
	 * CSCTL0_H register (password is 0xA5).
	 */

    	  CSCTL0_H = 0xA5;
    	  CSCTL1 |= DCOFSEL0 + DCOFSEL1;             // Set max. DCO setting - (default setting) - sets to 8 MHz
    	  CSCTL2 = SELA_0 + SELS_3 + SELM_3;        // Set the various clock sources: ACLK = XT1; MCLK and SMCLK = DCO
    	  CSCTL3 = DIVA_0 + DIVS_3 + DIVM_3;        // set all dividers - ACLK / 1; SMCLK and MCLK / 8 (so 1 MHz).
    	  CSCTL4 |= XT1DRIVE_0;						// set the XT1 oscillator current - lowest possible
    	  CSCTL4 &= ~XT1OFF;						// turns on the XT1 as long as XT1 is selected by port selection and not in bypass mode


    /*
     * Since the XT1 is an oscillator crystal, it takes a bit of time to settle down
     * to a stable cycle. Loop indefinitely, checking whether it faulted or not
     * until it didn't fault.
     */
    	  do
    	  {
    	    CSCTL5 &= ~XT1OFFG;						//This bit says whether or not there is a fault in the XT1 - so clear it
    	    SFRIFG1 &= ~OFIFG;						//This bit says whether there is a fault interrupt - so clear it
    	  }while (SFRIFG1&OFIFG);                   // Test to make sure there is no fault

    //That should do it for setting up the clock system control.
    //To summarize: ACLK is sourced from the DCO and has been divided down to 1 MHz.
    //MCLK and SMCLK are both sourced from the XT1 crystal and are at 32.768 kHz
}

void init_pins(void)
{
    //	  PJSEL0 |= BIT4 + BIT5; //LEDs?

    //	  // Configure UART pins
    //	  P2SEL1 |= BIT0 + BIT1;
    //	  P2SEL0 &= ~(BIT0 + BIT1);
}

void init_UART(void)
{
    /*
     * Setting a baud rate:
     *
     * According to p.488 of the user guide:
     *
     * To calculate the correct settings for the baudrate:
     * 1. Calculate N = f_brclk / baudrate
     * 		if N > 16, go to step 3, otherwise, go to step 2
     * 2. OS16 = 0, UCBRx = INT(N) (go to step 4)
     * 3. OS16 = 1, UCBRx = INT(N/16), UCBRFx = INT( [ (N/16) - INT(N/16) ] * 16 )
     * 4. UCBRSx can be found by looking up the fractional part of N (that is, N - INT(N)) in the table
     * 5. If OS16 = 0 was chosen, a detailed error calculation is recommended
     */

    /*
     * Let's set up a baud rate of 9600.
     *
     * First, find the f_brclk. We could use the internal DCO, though this is fairly inaccurate. It defaults to 1 MHz.
     * Otherwise, we could use the on-board crystal (32.768 kHz).
     *
     * Let's use the crystal.
     *
     * N = 32768 / 9600 = 3.41333, so:
     * OS16 = 0; UCBR0 = 3; and according to the table, UCBRS0 = 0x92;
     */



    /*
     * According to p.479 of the user guide:
     *
     * The recommended eUSCI_A initialization/reconfiguration
     * process is:
     * 1. Set UCSWRST bit (to avoid unpredictable behavior while configuring the USCI)
     * 2. Initialize all eUSCI_A registers including UCAxCTL1
     * 3. Configure ports
     * 4. Clear UCSWRST bit
     * 5. Enable interrupts via UCRXIE or UCTXIE
     */
    UCA0CTL1 |= UCSWRST;

	// Configure UART 0
//	  UCA0CTL1 = UCSSEL_1;                      // Set ACLK = 32768 as UCBRCLK
//	  UCA0BR0 = 3;
//	  UCA0BR1 = 0;
//	  UCA0MCTLW |= 0x5300;

    UCA0CTL1 &= ~UCSWRST;
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIA0TX_ISR(void)
{
	UCA0TXBUF = Data;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void)
{
	P1OUT = UCA0RXBUF;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	Data ^= 0x40;
	P1IFG = 0x00;
}
