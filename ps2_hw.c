#include "ps2_hw.h"

#include <util/delay.h>
#include <avr/interrupt.h>


enum ps2HwState {
    start   	=   0,
    data		=	1,
    parity  	=   9, 
    stop    	=   10,
    acknowledge	= 	11
};


enum ps2HwState    	state    = start;
volatile uint8_t	ps2HwFlags  = 0;
volatile uint8_t	ps2HwDataByte, temp;


uint8_t parity_control(uint8_t x){
    uint8_t loopCount = 8;
    uint8_t ps2ParityControl = 0;
    while(loopCount--){
        ps2ParityControl	^= 	x&1;
        x	<<=	1;
    }
   return ps2ParityControl;
}


uint8_t ps2_hw_get_flags( void ){
	return ps2HwFlags;
}



void ps2_hw_init( void ){
	EICRA |= 0x02;
	EIMSK |= 0x01;
	PS2_HW_CLK_DDR &= ~(1<<PS2_HW_CLK);
	PS2_HW_CLK_PORT |= (1<<PS2_HW_CLK); //enable pullup
	PS2_HW_DATA_DDR &= ~(1<<PS2_HW_DATA);
	PS2_HW_DATA_PORT |= (1<<PS2_HW_DATA); //enable pullup
	state  = start;
	ps2HwFlags = 0;
	
	

}



void ps2_hw_send_byte(uint8_t x){
	ps2HwFlags 	= PS2_HW_FLAG_SENDING;
	ps2HwDataByte = x;
	temp = x;
	PS2_HW_CLK_DDR |= (1<<PS2_HW_CLK);		//sets clk as output
	EIMSK &= (0x00);						
	PS2_HW_CLK_PORT &= ~(1<<PS2_HW_CLK);	//sets clk low
	_delay_us( 100 );
	PS2_HW_DATA_DDR |= (1<<PS2_HW_DATA); //sets data output
	EIMSK |= 0x01;
	PS2_HW_DATA_PORT &= ~(1<<PS2_HW_DATA);
	PS2_HW_CLK_DDR &= ~(1<<PS2_HW_CLK);
	PS2_HW_CLK_PORT |= (1<<PS2_HW_CLK); //enable pullup
	
	
	
	
}


int8_t ps2_hw_receive_byte(uint8_t *x){
	if(ps2HwFlags & PS2_HW_FLAG_RCV_COMPLETE){
        ps2HwFlags &= ~PS2_HW_FLAG_RCV_COMPLETE;
        (*x) = ps2HwDataByte;
		if(!(ps2HwFlags & PS2_HW_FLAG_ERROR)){
			return 0;
		}
		else{
			return -2;
		}
	}
	else{
		return -1;
	}
		
}



ISR( INT0_vect )
{

    if(!(ps2HwFlags & PS2_HW_FLAG_SENDING)){
        switch(state){
            case start:
                if(!(PS2_HW_DATA_PIN & (1<<PS2_HW_DATA))){	// überprüft startbit = 0 
                    state 		=	data;
                    ps2HwDataByte	=	0;
                }
                break;

            case parity:
                if((PS2_HW_DATA_PIN & 1<<PS2_HW_DATA)&parity_control(ps2HwDataByte)){
                    ps2HwFlags	|=	PS2_HW_FLAG_ERROR ;
                }
                state	=	stop;
                break;

            case stop:
			  ps2HwFlags	&=	~(PS2_HW_FLAG_RECEIVING);
                if(PS2_HW_DATA_PIN & (1<<PS2_HW_DATA)){	// prüft stopbit = 1
                    ps2HwFlags	|=	PS2_HW_FLAG_RCV_COMPLETE;
                }else{
                    ps2HwFlags	|=	PS2_HW_FLAG_ERROR;
                  
                }
                state 	=	start;

                break;

            default:
                ps2HwDataByte >>= 1;
                if(PS2_HW_DATA_PIN & (1<<PS2_HW_DATA)){ 
                    ps2HwDataByte |= (1<<7);
                }else{
                    ps2HwDataByte &= ~((1<<7));
                }
                state++;
                ps2HwFlags  |=	PS2_HW_FLAG_RECEIVING;
                break;
        }


    }else{
        switch(state){
            case parity:
				if(!parity_control(temp)){
					PS2_HW_DATA_PORT |= (1<<PS2_HW_DATA);
				}
				else{
					PS2_HW_DATA_PORT &= ~(1<<PS2_HW_DATA);
				}
				state	=	stop;
                break;

            case stop:
                PS2_HW_DATA_DDR &= ~(1<<PS2_HW_DATA);
				PS2_HW_DATA_PORT |= (1<<PS2_HW_DATA);
                state = acknowledge;
                break;

            case acknowledge:

                if(PS2_HW_DATA_PIN &(1<<PS2_HW_DATA)){
                    ps2HwFlags	|=	PS2_HW_FLAG_ERROR; 
                }else{
                    ps2HwFlags  |= PS2_HW_FLAG_TRANSF_COMPLETE;

                }
                ps2HwFlags	&=	~(PS2_HW_FLAG_SENDING);
                state = start;
                break;

            case data:

                if(ps2HwDataByte&1){
                    PS2_HW_DATA_PORT &= ~(1<<PS2_HW_DATA);
                }else{
                    PS2_HW_DATA_PORT |= (1<<PS2_HW_DATA);
                }
                state++;
                ps2HwFlags	|=	PS2_HW_FLAG_SENDING; 
                ps2HwDataByte >>= 1;
                break;
        }
    }		

}
