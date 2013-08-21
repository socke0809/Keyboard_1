#include <avr/interrupt.h>

#define PS2_FLAG_RCV_COMPLETE_1 1
#define PS2_FLAG_RCV_COMPLETE_2 (1<<2)
#define PS2_FLAG_RCV_COMPLETE_3 (1<<3)
#define PS2_FLAG_RCV_COMPLETE_4 (1<<4)
#define PS2_FLAG_RCV_COMPLETE_5 (1<<5)
#define PS2_FLAG_RCV_COMPLETE_6 (1<<6)
#define PS2_FLAG_RCV_COMPLETE_7 (1<<7)
#define PS2_FLAG_RCV_COMPLETE_8 (1<<8)

enum ps2ReceiveState {
	start   =   0,
	data	=	1,
	parity  =   9, 
	stop    =   10
};

enum ps2ReceiveState state = start;
char PS2_FLAG_RCV_COMPLETE;

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
