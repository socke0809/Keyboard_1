#include <avr/interrupt.h>
#include "main.h"

#define PS2_DATA		PD1
#define PS2_DATA_PIN	PIND
#define PS2_DATA_DDR	DDRD
#define PS2_DATA_PORT	PORTD

#define PS2_CLK			PD2
#define PS2_CLK_PIN		PIND
#define PS2_CLK_DDR		DDRD
#define PS2_CLK_PORT	PORTD



enum ps2ReceiveState    state       = start;
volatile uint8_t        ps2FlagRcv  = 0;
volatile uint8_t 		ps2RcvDataByte;
volatile uint8_t		ps2ParityControl = 0;




ISR( INT0_vect )
{
	int loopCount = 8;
	switch(state){
		case start:
			if(!(PS2_DATA_PIN & (1<<PS2_DATA))){ // überprüft startbit = 0 
				state 			=	data;
				ps2RcvDataByte	=	0;
			}
			break;

		case parity:
			
			while(loopCount--){
			
				ps2ParityControl ^= ps2RcvDataByte&1;
				ps2RcvDataByte <<= 1;
			}
			if(PS2_DATA != ps2ParityControl){
				ps2FlagRcv	=	PS2_FLAG_RCV_ERROR ;
			}
			else{
				state =	stop;
			}
			break;

		case stop:
            if(PS2_DATA_PIN & (1<<PS2_DATA)){	//prüft stopbit = 1
				ps2FlagRcv		=	PS2_FLAG_RCV_COMPLETE;
				state = start;
			}
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
