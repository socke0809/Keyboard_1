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

struct ps2Buffer{
	uint8_t buffer[PS2_BUFFER_SIZE];
	
	uint8_t	read;
	uint8_t	write;
	uint8_t	ps2BufFlags;
};
	
enum ps2HwState    	state = start;
volatile uint8_t	isrBit = 0;
volatile uint8_t	ps2HwFlags = 0;
volatile uint8_t	ps2HwDataByte;
volatile uint8_t	temp;
volatile struct ps2Buffer	sendBuffer;
volatile struct ps2Buffer	rcvBuffer;

int8_t ps2_buffer_write(uint8_t data, struct ps2Buffer *buf){
	if((buf->write==buf->read) && (!(buf->ps2BufFlags & PS2_BUFFER_EMPTY))){//buffer full
		isrBit = 1;
		buf->ps2BufFlags = PS2_BUFFER_FULL;
		isrBit = 0;
		return -1;
	}
	buf->buffer[buf->write] = data;
	buf->write++;
	buf->ps2BufFlags &= ~(PS2_BUFFER_EMPTY);
	if(buf->write == PS2_BUFFER_SIZE){
		buf->write = 0;
	}
	return 0;
}

int8_t ps2_buffer_read(uint8_t *data, struct ps2Buffer *buf){
	if((buf->read == buf->write) && (!(buf->ps2BufFlags & PS2_BUFFER_FULL))){ //buffer empty
		isrBit = 1;
		buf->ps2BufFlags = PS2_BUFFER_EMPTY;
		isrBit = 0;
		return -1;
	}
	(*data) = buf->buffer[buf->read];
	buf->read++;
	buf->ps2BufFlags &= ~(PS2_BUFFER_FULL);
	
	if(buf->read == PS2_BUFFER_SIZE){
			buf->read = 0;
	}
	return 0;
}
	


uint8_t parity_control(uint8_t x){
    uint8_t ps2ParityControl = 0;
    for(uint8_t i = 0; i<8; i++){
        ps2ParityControl	^= 	x&1;
        x	>>=	1;
    }
    return ps2ParityControl;
}


uint8_t ps2_hw_get_flags( void ){
    return ps2HwFlags;
}

uint8_t ps2_get_buf_flags(void){
return rcvBuffer.ps2BufFlags;
}



void ps2_hw_init( void ){
    PS2_HW_CLK_DDR &= ~(1<<PS2_HW_CLK);
    PS2_HW_CLK_PORT |= (1<<PS2_HW_CLK); //enable pullup
    PS2_HW_DATA_DDR &= ~(1<<PS2_HW_DATA);
    PS2_HW_DATA_PORT |= (1<<PS2_HW_DATA); //enable pullup
    state  = start;
    ps2HwFlags = 0;
	
	rcvBuffer.ps2BufFlags =  PS2_BUFFER_EMPTY;
	sendBuffer.ps2BufFlags =  PS2_BUFFER_EMPTY;
	rcvBuffer.read = 0;
	rcvBuffer.write = 0;
	sendBuffer.read = 0;
	sendBuffer.write = 0;
	

    //enable interrupt
    EICRA |= 0x02; //TODO
    EIMSK |= 0x01; //TODO
}



int8_t ps2_hw_send_byte(uint8_t data){
	int8_t x = ps2_buffer_write(data, &sendBuffer);
	
	if(!(ps2HwFlags & PS2_HW_FLAG_RECEIVING)){
		PS2_HW_CLK_DDR |= (1<<PS2_HW_CLK);//clk as output
		PS2_HW_CLK_PORT &= ~(1<<PS2_HW_CLK);//clk low
		 _delay_us(150);
		 PS2_HW_CLK_DDR &= ~(1<<PS2_HW_CLK);//clk as input
		 PS2_HW_CLK_PORT |= (1<<PS2_HW_CLK); //enable pullup
		 
	}
	
    return x;
}


int8_t ps2_hw_receive_byte(uint8_t *data){
		return ps2_buffer_read(data, &rcvBuffer);
		/*if(!(ps2HwFlags & PS2_HW_FLAG_ERROR)){
			if(temp1 == 0){
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
	else{
	return -3;
	}*/
}



ISR( INT0_vect )
{	
	if(isrBit != 1){
    if(!(ps2HwFlags&PS2_HW_FLAG_SENDING)){

        switch(state){
            case start:
			
				if(!(sendBuffer.ps2BufFlags & PS2_BUFFER_EMPTY)){
					ps2HwFlags |= PS2_HW_FLAG_SENDING;
					break;
					}
				else if((PS2_HW_DATA_PIN & (1<<PS2_HW_DATA)) == 0){	// überprüft startbit = 0 
					state 		=	data;
					ps2HwDataByte	=	0;
					//ps2HwFlags	&=	~PS2_HW_FLAG_RCV_COMPLETE;
					ps2HwFlags |= PS2_HW_FLAG_RECEIVING;
                }
                break;


            case parity:
                if(
				    ( (PS2_HW_DATA_PIN & (1<<PS2_HW_DATA)) &&  parity_control(ps2HwDataByte)) ||
                    (!(PS2_HW_DATA_PIN & (1<<PS2_HW_DATA)) && !parity_control(ps2HwDataByte))
				){
                    ps2HwDataByte = 0;
					state = start;
				}
                else{
                    state	=	stop;
                }
               
                break;


            case stop:
				temp  = ps2_buffer_write(ps2HwDataByte, &rcvBuffer);
                if(((PS2_HW_DATA_PIN & (1<<PS2_HW_DATA)) == 0)||(temp!=0)){	// prüft stopbit = 1
                    ps2HwFlags	|=	PS2_HW_FLAG_ERROR;

                }
				ps2HwFlags &= ~PS2_HW_FLAG_RECEIVING;
               ps2HwFlags	|=	PS2_HW_FLAG_RCV_COMPLETE;
				state 	=	start;
				if(!(sendBuffer.ps2BufFlags & PS2_BUFFER_EMPTY )){
					PS2_HW_CLK_DDR |= (1<<PS2_HW_CLK);//clk as output
					PS2_HW_CLK_PORT &= ~(1<<PS2_HW_CLK);//clk low
					 _delay_us(150);
					PS2_HW_CLK_DDR &= ~(1<<PS2_HW_CLK);//clk as input
					PS2_HW_CLK_PORT |= (1<<PS2_HW_CLK); //enable pullup
					
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
    }
	else{
        switch(state){
            case start:
				ps2HwFlags |= PS2_HW_FLAG_SENDING;
				ps2_buffer_read(&ps2HwDataByte, &sendBuffer);
				
				temp = ps2HwDataByte;
					PS2_HW_DATA_DDR |= (1<<PS2_HW_DATA); //sets data output
					PS2_HW_DATA_PORT &= ~(1<<PS2_HW_DATA);
				//EIMSK |= 0x01;
				state = data;

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
                //ps2HwFlags  |= PS2_HW_FLAG_TRANSF_COMPLETE;
                ps2HwFlags	&=	~(PS2_HW_FLAG_SENDING);
				if(!(sendBuffer.ps2BufFlags & PS2_BUFFER_EMPTY)){
					PS2_HW_CLK_DDR |= (1<<PS2_HW_CLK);//clk as output
					PS2_HW_CLK_PORT &= ~(1<<PS2_HW_CLK);//clk low
					 _delay_us(150);
					PS2_HW_CLK_DDR &= ~(1<<PS2_HW_CLK);//clk as input
					PS2_HW_CLK_PORT |= (1<<PS2_HW_CLK); //enable pullup
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
}//if isrBit
}
