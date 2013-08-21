#include <avr/interrupt.h>
#include "main.h"


enum ps2ReceiveState    state       = start;
volatile uint8_t        ps2FlagRcv  = 0;
volatile uint8_t 		ps2RcvDataByte;

//PD2- Pin 2 am arduino - clk
//PD1- pin 1 am arduino - data 
// in PORTB sollen die gesendeten bit rein 


ISR( INT0_vect )
{
	switch(state){
		case start:
			if(!(PIND & (1<<PD1))){ // überprüft startbit = 0 
				state 			=	data;
				ps2RcvDataByte	=	0;
			}
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
        if(ps2FlagRcv){ // TODO: check if transfer complete and no error occured
            //TODO Translate
        }
    }
}
