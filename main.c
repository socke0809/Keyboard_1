#include <avr/interrupt.h>

#define PS2_FLAG_RCV_COMPLETE 1
#define PS2_FLAG_CORRECT_PARITY 1

enum ps2ReceiveState {
	start   =   0,
	data	=	1,
	parity  =   9, 
	stop    =   10
};

enum ps2ReceiveState state = start;
volatile uint8_t PS2_FLAG;

ISR( INT0_vect )
{
	switch(state){
		case start:
			//TODO 0 übertragen
			state =	data;
			break;
		case parity:
			//TODO parity
				// wenn parity ist falsch
				PS2_FLAG_CORRECT_PARITY = 0;
			state =	stop;
			break;
		case stop:
			PS2_FLAG_RCV_COMPLETE = 1;
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
	sei();
	
	
    while(1){
	if(PS2_FLAG_CORRECT_PARITY){
	//TODO Translate
	}
}
