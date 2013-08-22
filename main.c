#include <avr/interrupt.h>
#include "main.h"





enum ps2ReceiveState    state       = start;
volatile uint8_t        ps2RcvFlag  = 0;
volatile uint8_t        ps2RcvDataByte;





ISR( INT0_vect )
{
	int loopCount = 8;
	int ps2ParityControl = 0;
	int bitCount = 0;
	switch(state){
		case start:
			if(!(PS2_DATA_PIN & (1<<PS2_DATA))){	// überprüft startbit = 0 
				state 		=	data;
				ps2RcvDataByte	=	0;
			}
			break;

		case parity:
			
			while(loopCount--){
				ps2ParityControl	^= 	ps2RcvDataByte&1;
				ps2RcvDataByte		<<=	1;
			}
			if(PS2_DATA != ps2ParityControl){
				ps2RcvFlag	|=	PS2_RCV_FLAG_ERROR ;
			}
				state	=	stop;
			break;

		case stop:
            if(PS2_DATA_PIN & (1<<PS2_DATA)){	// prüft stopbit = 1
					ps2RcvFlag	|=	PS2_RCV_FLAG_COMPLETE;
					state        =	start;
			}
			break;
            
		default:
			if(PS2_DATA_PIN & (1<<PS2_DATA)){ 
				ps2RcvDataByte |= (1<<bitCount);
				}
			else{
				ps2RcvDataByte &= ~(1<<bitCount);
				}
			bitCount++;
			state++;
			ps2RcvFlag  |=	PS2_RCV_FLAG_RECEIVING;
			break;

	}
}


int main()
{
	sei();


    while(1){
        if(ps2RcvFlag){ // TODO: check if transfer complete and no error occured 
            //TODO Translate
        }
    }
}
