#include <avr/interrupt.h>
#include "main.h"


enum ps2ReceiveState    state       = start;
volatile uint8_t        ps2FlagRcv  = 0;


ISR( INT0_vect )
{
	switch(state){
		case start:
			//TODO prüfen ob 0 übertragen wurde
			state =	data;
			break;

		case parity:
			//TODO parity check
				//TODO: wenn parity ist falsch
                //TODO: setze error flag
			state =	stop;
			break;

		case stop:
            //TODO: prüfen ob 1 übertragen wurde
            //TODO: setze complete flag
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
        if( 1 ){ // TODO: check if transfer complete and no error occured
            //TODO Translate
        }
    }
}
