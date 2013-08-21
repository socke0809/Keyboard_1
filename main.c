#include <avr/interrupt.h>

enum ps2ReceiveState = {
	start   =   0, 
	parity  =   9, 
	stop    =   10
};

enum ps2ReceivesState state;


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
			ps2ReceiveState++;
			break;
	}
	
}
}
	
	


int main()
{
    while(1);
}
