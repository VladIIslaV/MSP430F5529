#include <msp430.h>

int main(void)
{
	WDRCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	volatile unsigned int i;	// 
	P1DIR &= !(BIT7);
	P8DIR |= BIT2;
	
	P1OUT |= BIT7;	// ?????????????
	P1IE |= BIT7;	// interrupt enable
	P1IES |= BIT7;	// edge for interrupt: high to low

		// Enter LPM0, enable interrupt
	__bit_SR_register(LPM0_bits + GIE);
	__no_operations();	// For debugger
	return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_IST(void)
{
	if(P1IN & BIT7)	// High leverl - don't press button
	{
		P8OUT &= !(BIT2);	// Off led
	}
	else
	{
		P1OUT |= BIT2;	// low level- press button- on lled
	}
	if(!(P1IES & BIT7))	// ?????
	{
		P1REN ^= BIT7;	// ????????
	}
	P1IES ^= BIT7;	// ?????
	P1IFG = 0;		// reset interrupt flag
}
