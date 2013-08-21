#include <avr/interrupt.h>

enum ps2ReceiveState {
	start   =   0, 
	parity  =   9, 
	stop    =   10
};

enum ps2ReceiveState state = start;


ISR( INT0_vect )
{
	switch(state){
		case start:
			//TODO 0 übertragen
			state++;
			break;
		case parity:
			//TODO parity
			state++;
			break;
		case stop:
			state = 0;
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
