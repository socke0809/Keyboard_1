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
volatile uint8_t 	ps2RcvBuffer[rcvBufSize];
volatile uint8_t 	ps2SendBuffer[sendBufSize];
volatile uint8_t	rcvBufRead;
volatile uint8_t	sendBufRead;
volatile uint8_t	rcvBufWrite;
volatile uint8_t	sendBufWrite;
volatile uint8_t	ps2BufFlags = 0;



/*int8_t ps2_buffer_write(uint8_t data){
	if((write==read)&&(!(ps2BufFlags&PS2_BUFFER_EMPTY))){//buffer full
		ps2BufFlags = PS2_BUFFER_FULL;
		return -1;
	}
	ps2Buffer[write] = data;
	write++;
	ps2BufFlags &= ~(PS2_BUFFER_EMPTY);
	if(write == size){
		write = 0;
	}
	return 0;
}

int8_t ps2_buffer_read(uint8_t *x){

	if(read == write)&&(!(ps2BufFlags&PS2_BUFFER_FULL)){ //buffer empty
		ps2BufFlags = PS2_BUFFER_EMPTY;
		return -1;
	}
	(*x) = ps2[read];
	read++;
	ps2BufFlags &= ~(PS2_BUFFER_FULL)
	
	if(read == size){
			read = 0;
	}
	return 0;
}
*/		


uint8_t parity_control(uint8_t x){
    uint8_t ps2ParityControl = 0;
    for(uint8_t i = 0; i<8; i++){
        ps2ParityControl	^= 	x&1;
        x	>>=	1;
    }
    return ps2ParityControl;
}


uint8_t ps2_HW_get_flags( void ){
    return ps2HwFlags;
}



void ps2_hw_init( void ){
    PS2_HW_CLK_DDR &= ~(1<<PS2_HW_CLK);
    PS2_HW_CLK_PORT |= (1<<PS2_HW_CLK); //enable pullup
    PS2_HW_DATA_DDR &= ~(1<<PS2_HW_DATA);
    PS2_HW_DATA_PORT |= (1<<PS2_HW_DATA); //enable pullup
    state  = start;
    ps2HwFlags = 0;

    //enable interrupt
    EICRA |= 0x02; //TODO
    EIMSK |= 0x01; //TODO
	
	sendBufRead 	= 0;
	sendBufWrite	= 0;
	rcvBufRead 		= 0;
	rcvBufWrite 	= 0;
	ps2BufFlags 	= PS2_SEND_BUFFER_EMPTY;	
	ps2BufFlags 	|= PS2_RCV_BUFFER_EMPTY;
}



uint8_t ps2_send_byte(uint8_t data){
    if((sendBufWrite == sendBufRead) && (!(ps2BufFlags & PS2_SEND_BUFFER_EMPTY))){//buffer full
		ps2BufFlags |= PS2_SEND_BUFFER_FULL;
		return -1;
	}
	ps2SendBuffer[sendBufWrite] = data;
	sendBufWrite++;
	ps2BufFlags &= ~(PS2_SEND_BUFFER_EMPTY);
	if(sendBufWrite == sendBufSize){
		sendBufWrite = 0;
	}
	return 0;
}



int8_t ps2_receive_byte(uint8_t *x){
    if(ps2HwFlags & PS2_HW_FLAG_RCV_COMPLETE){
       if((rcvBufRead == rcvBufWrite) && (!(ps2BufFlags & PS2_RCV_BUFFER_FULL))){ //buffer empty
		ps2BufFlags = PS2_RCV_BUFFER_EMPTY;
		return -2;
		}
		(*x) = ps2RcvBuffer[rcvBufRead];
		rcvBufRead++;
		ps2BufFlags &= ~((PS2_RCV_BUFFER_FULL));
	
		if(rcvBufRead == rcvBufSize){
			rcvBufRead = 0;
		}
		if(!(ps2HwFlags & PS2_HW_FLAG_ERROR)){
            return 0;
		}
	}
    else{
            return -1;
    }
    
}



ISR( INT0_vect )
{
    if((!(ps2BufFlags & PS2_SEND_BUFFER_EMPTY)) && (state == start)){

        switch(state){
            case start:
                if((PS2_HW_DATA_PIN & (1<<PS2_HW_DATA)) == 0){	// überprüft startbit = 0 
                    state 		=	data;
                    ps2HwDataByte	=	0;
                }
                break;


            case parity:
                if(
				((PS2_HW_DATA_PIN & (1<<PS2_HW_DATA)) && parity_control(ps2HwDataByte)) || (!(PS2_HW_DATA_PIN & (1<<PS2_HW_DATA)) && !parity_control(ps2HwDataByte))
				){
                    ps2HwFlags	|=	PS2_HW_FLAG_ERROR;
				}
                else{
                    ps2HwFlags  &=~ PS2_HW_FLAG_ERROR;
                }
                state	=	stop;
                break;


            case stop:
                if((PS2_HW_DATA_PIN & (1<<PS2_HW_DATA)) == 0){	// prüft stopbit = 1
                    ps2HwFlags	|=	PS2_HW_FLAG_ERROR;

                }
                ps2HwFlags	|=	PS2_HW_FLAG_RCV_COMPLETE;
                state 	=	start;
				if((rcvBufWrite == rcvBufRead) && (!(ps2BufFlags & PS2_RCV_BUFFER_EMPTY))){
					ps2BufFlags |= PS2_RCV_BUFFER_FULL;
					break;
				}
				ps2RcvBuffer[rcvBufWrite] = ps2HwDataByte;
				rcvBufWrite++;
				ps2BufFlags &= ~(PS2_RCV_BUFFER_EMPTY);
				if(rcvBufWrite == rcvBufSize){
					rcvBufWrite = 0;
				}
                break;


            default:
                ps2HwDataByte >>= 1;
                if(PS2_HW_DATA_PIN & (1<<PS2_HW_DATA)){ 
                    ps2HwDataByte |= 0x80;
                }else{
                    ps2HwDataByte &= ~0x80;
                }
                state++;
                break;
        } //switch
    }else{
        switch(state){
            case start:
				EIMSK &= ~(0x01);						
					PS2_HW_CLK_DDR |= (1<<PS2_HW_CLK);		//sets clk as output
					PS2_HW_CLK_PORT &= ~(1<<PS2_HW_CLK);	//sets clk low

					_delay_us( 150 );

					PS2_HW_CLK_DDR &= ~(1<<PS2_HW_CLK);
					PS2_HW_CLK_PORT |= (1<<PS2_HW_CLK); //enable pullup

					PS2_HW_DATA_DDR |= (1<<PS2_HW_DATA); //sets data output
					PS2_HW_DATA_PORT &= ~(1<<PS2_HW_DATA);
				EIMSK |= 0x01;
				state = data;
				
				
				ps2HwDataByte = ps2SendBuffer[sendBufRead];
				uint8_t temp  = ps2SendBuffer[sendBufRead];
				sendBufRead++;
				ps2BufFlags &= ~(PS2_SEND_BUFFER_FULL);
	
				if(sendBufRead == sendBufSize){
					sendBufRead = 0;
				}

                break;


            case parity:
                if(parity_control(temp)){   //mind this: parity_control is inverse.
                    PS2_HW_DATA_PORT &= ~(1<<PS2_HW_DATA);
                }
                else{
                    PS2_HW_DATA_PORT |= (1<<PS2_HW_DATA);
                }
                state	=	stop;
                break;


            case stop:
                PS2_HW_DATA_DDR &= ~(1<<PS2_HW_DATA); //release pin
                PS2_HW_DATA_PORT |= (1<<PS2_HW_DATA); //enable pullups
                state = acknowledge;
                break;


            case acknowledge:
                if(PS2_HW_DATA_PIN & (1<<PS2_HW_DATA)){ //should pull ACK low
                    ps2HwFlags	|= PS2_HW_FLAG_ERROR; 
                }else{
                    ps2HwFlags	&=	~PS2_HW_FLAG_ERROR; 
                }
                ps2HwFlags  |= PS2_HW_FLAG_TRANSF_COMPLETE;
                ps2HwFlags	&=	~(PS2_HW_FLAG_SENDING);
				if((sendBufRead == sendBufWrite) && (!(ps2BufFlags & PS2_SEND_BUFFER_FULL))){ //buffer empty
					ps2BufFlags = PS2_SEND_BUFFER_EMPTY;
				}
                state = start;
                break;


            default:
                if(ps2HwDataByte&1){
                    PS2_HW_DATA_PORT |= (1<<PS2_HW_DATA);
                }else{
                    PS2_HW_DATA_PORT &= ~(1<<PS2_HW_DATA);
                }
                state++;
                ps2HwDataByte >>= 1;
                break;
        } //switch
    }
}
