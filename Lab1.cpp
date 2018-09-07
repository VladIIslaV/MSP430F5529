#include <msp430.h>

void delay(int numb)
{
	volatile int i = 0;
	while(i < numb)
	{
		i++;
	}
}

void Initialization()
{
	WDTCTL = WDTPW+WDTHOLD;                   // Stop WDT

	P1OUT = 0;
	P1DIR |= BIT0;

	P8OUT = 0;
	P8DIR |= BIT1;
	P8DIR |= BIT2;

	P1REN |= BIT7;
	P1OUT |= BIT7;
	P1DIR &= ~BIT7;

}

#define LedOn(port,bit) \
	do{\
		(P##port##OUT |= BIT##bit);\
	}while(0)

#define LedOff(port,bit) \
	do{\
		(P##port##OUT &= ~BIT##bit);\
	}while(0)

#define LedReverse(port,bit) \
	do{\
		(P##port##OUT ^= BIT##bit);\
	}while(0)

#define IsPressed(port,bit) ((P##port##IN & BIT##bit) == 0)

int IsButtonS1Pressed()
{
	return ((P1IN & BIT7) == 0);
}

int main(void)
{
	Initialization();

	volatile int ledCnt = 0;

	while(1)
	{
		if(IsPressed(1, 7))
		{
			ledCnt++;
			if(ledCnt == 1)
			{
				LedOn(1, 0);
			}
			else
			{
				if(ledCnt == 2)
				{
					LedOn(8, 1);
				}
				else
				{
					if(ledCnt == 3)
					{
						LedOn(8, 2);
					}
					else
					{
						ledCnt = 0;
						LedOff(1, 0);
						LedOff(8, 1);
						LedOff(8, 2);
					}
				}
			}

			while((P1IN & BIT7) == 0);
		}
		delay(1000);
	}

}
