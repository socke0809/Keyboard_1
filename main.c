#include <avr/interrupt.h>

enum ps2ReceiveState {
	start   =   0,
	data	=	1,
	parity  =   9, 
	stop    =   10
};

enum ps2ReceiveState state = start;


ISR( INT0_vect )
{
	switch(state){
		case start:
			//TODO 0 übertragen
			state =	data;
			break;
		case parity:
			//TODO parity
			state =	stop;
			break;
		case stop:
			state = start;
			break;
		default:
			//TODO: read data
			state++;
			break;
	}
	
}
	
	


int main()
{
    while(1);
}
