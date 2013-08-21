#include <avr/interrupt.h>

enum ps2ReceiveState = {
	start   =   0, 
	parity  =   9, 
	stop    =   10
};


ISR( INT0_vect )
{
}


int main()
{
    while(1);
}
