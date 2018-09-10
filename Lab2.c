// Variant 15

#include <msp430.h>

#define LedReverse(port,bit) \
	do{\
		(port ^= bit);\
	}while(0)

#define LedOn(port,bit) \
	do{\
		(port |= bit);\
	}while(0)

int main(void)
{
  WDTCTL = WDT_MDLY_32;                     // WDT 32ms, SMCLK, interval timer 
  SFRIE1 |= WDTIE;                          // Enable WDT interrupt

  P1OUT = 0;	//connected to leds

  P1OUT |= BIT7;	//button S1 init
  P1DIR &= ~BIT7;
  P1REN |= BIT7;

  P1IES |= BIT7;	// button S1 irq direction init

  P1DIR |= 0x01;                            // Set P1.0 to output direction
  P1IE |= BIT7;								// Enable S1 interrupt

  P1DIR |= 0x3E;	// Should be rewritten


  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
  __no_operation();                         // For debugger
}

volatile int curLed = 1;
volatile const int timer = 25;	// 25*32ms

volatile static int ledTimer[6];

// Watchdog Timer interrupt service routine
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
  //P1OUT ^= 0x01;
	int i = 1;// Toggle P1.0 (LED)
	for(i; i <= 5; i++)
	{
		if(P1OUT&(BIT0<<i))
		{
			if(!ledTimer[i])
			{
				ledTimer[i] = timer;
			}
			ledTimer[i]--;
			if(!ledTimer[i])
			{
				P1OUT &= ~(BIT0<<i);
			}
		}
	}
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	LedOn(P1OUT, 0x01 << curLed);
	ledTimer[curLed] = 0;
	P1IES ^= BIT7;
	curLed = curLed % 5 + 1;
	P1IFG &= ~BIT7;
}
