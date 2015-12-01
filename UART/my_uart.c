#include "my_uart.h"
#include <msp430.h>


void init_uart(void)
{
	CSCTL0_H = 0xA5;                          // Unlock register
	CSCTL1 |= DCOFSEL0 + DCOFSEL1;            // Set max. DCO setting
	CSCTL2 = SELA_1 + SELS_3 + SELM_3;        // set ACLK = vlo; MCLK = DCO
	CSCTL3 = DIVA_0 + DIVS_0 + DIVM_0;        // set all dividers
	CSCTL0_H = 0x01;                          // Lock Register

	// Configure UART pins P2.0 & P2.1
	P2SEL1 |= BIT0 + BIT1;
	P2SEL0 &= ~(BIT0 + BIT1);
	P2DIR |= BIT0;

	// Configure UART 0
	UCA0CTL1 |= UCSWRST;
	UCA0CTL1 = UCSSEL_2;                      // Set SMCLK as UCLk
	UCA0BR0 = 52;                            // 9600 baud
	UCA0BR1 = 0;
	UCA0MCTLW = 0x4911 ;

	UCA0CTL1 &= ~UCSWRST;                     // release from reset


//	  PJSEL0 |= BIT4 + BIT5;
//
//	  CSCTL0_H = 0xA5;
//	  CSCTL1 |= DCOFSEL0 + DCOFSEL1;             // Set max. DCO setting
//	  CSCTL2 = SELA_0 + SELS_3 + SELM_3;        // set ACLK = XT1; MCLK = DCO
//	  CSCTL3 = DIVA_0 + DIVS_3 + DIVM_3;        // set all dividers
//	  CSCTL4 |= XT1DRIVE_0;
//	  CSCTL4 &= ~XT1OFF;
//
//	  do
//	  {
//	    CSCTL5 &= ~XT1OFFG;
//	                                            // Clear XT1 fault flag
//	    SFRIFG1 &= ~OFIFG;
//	  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
//
//	  // Configure UART pins
//	  P2SEL1 |= BIT0 + BIT1;
//	  P2SEL0 &= ~(BIT0 + BIT1);
//	  // Configure UART 0
//	  UCA0CTL1 |= UCSWRST;
//	  UCA0CTL1 = UCSSEL_1;                      // Set ACLK = 32768 as UCBRCLK
//	  UCA0BR0 = 3;                              // 9600 baud
//	  UCA0BR1 = 0;
//	  UCA0MCTLW |= 0x5300;                      // 32768/9600 - INT(32768/9600)=0.41
//	                                            // UCBRSx value = 0x53 (See UG)
//	  UCA0CTL1 &= ~UCSWRST;                     // release from reset
}

void uart_write(unsigned char c)
{
	UCA0TXBUF = c;
}
