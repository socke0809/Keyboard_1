#include <avr/interrupt.h>
#include <util/delay.h>
#include "main.h"





enum ps2State    state    = start;
volatile uint8_t        ps2Flag  = 0;
volatile uint8_t        ps2DataByte;


void ps2_send_byte(){
	ps2Flag 	= PS2_FLAG_SENDING;
	PS2_CLK_DDR |= (1<<PS2_CLK); //sets clk as output 
	PS2_CLK_PORT &= ~(1<<PS2_CLK); //sets clk low
	_delay_us( 100 );
	PS2_DATA_DDR |= (1<<PS2_DATA); //sets data output
	PS2_DATA_PORT &= ~(1<<PS2_DATA);
	PS2_CLK_DDR &= ~(1<<PS2_CLK);

	
	
}

void ps2_receive_byte(uint8_t x){
if()
	
}


ISR( INT0_vect )
{
	int loopCount = 8;
	int ps2ParityControl = 0;
	if(ps2Flag 	!= PS2_FLAG_SENDING){
		switch(state){
			case start:
				if(!(PS2_DATA_PIN & (1<<PS2_DATA))){	// überprüft startbit = 0 
					state 		=	data;
					ps2DataByte	=	0;
				}
				break;

			case parity:
			
				while(loopCount--){
					ps2ParityControl	^= 	ps2DataByte&1;
					ps2DataByte		<<=	1;
				}
				if(PS2_DATA != ps2ParityControl){
					ps2Flag	|=	PS2_FLAG_ERROR ;
				}
				state	=	stop;
				break;

			case stop:
				if(PS2_DATA_PIN & (1<<PS2_DATA)){	// prüft stopbit = 1
						ps2Flag	|=	PS2_FLAG_COMPLETE;
						ps2Flag	&=	~(PS2_FLAG_RECEIVING);
				}
				else{
					ps2Flag	|=	PS2_FLAG_ERROR;
					ps2Flag	&=	~(PS2_FLAG_RECEIVING);
				}
				state        =	start;
			
				break;
            
			default:
				ps2DataByte >>= 1;
				if(PS2_DATA_PIN & (1<<PS2_DATA)){ 
					ps2DataByte |= (1<<7);
				}
				else{
					ps2DataByte &= ~((1<<7));
				}
				state++;
				ps2Flag  |=	PS2_FLAG_RECEIVING;
				break;
		}
	}
	else{
		
	}
				
	
				
}


int main()
{	
	
	sei();


    while(1){
        if(ps2Flag & PS2_FLAG_COMPLETE){ //check if transfer complete 
			if(!(ps2Flag & PS2_FLAG_ERROR)){ //and no error occured 
            //TODO Translate
			}
        }
    }
}
