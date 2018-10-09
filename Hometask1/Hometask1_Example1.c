#include <msp430.h>

int main(void)
{
	WDRCTL = WDTPW | WDTHOLD;
	volatile unsigned int i;
	P1DIR &= !(BIT7);
	P8DIR |= BIT2;
	while(1)
	{
		if(P1IN & BIT7)
		{
			P8OUT &= !(BIT2);
		}
		else
		{
			P8OUT |= BIT2;
		}
		for(i = 25000; i > 0; i--);
		P8OUT &= !(BIT2);
		for(i = 25000; i > 0; i--);
	}
	return 0;
}
