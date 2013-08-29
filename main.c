#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ps2_hw.h"
#include "uart.h"


int main( void )
{
    uart_init();
	
	ps2_hw_init();

	sei();
	
	_delay_ms(1000); //dbg


    uart_send_string("---");// WELCOME TO PS2 CONTROLLER ---\n");
	
	uart_send_byte( ps2_hw_get_flags() );
	
	
	
	
	ps2_hw_send_byte(0xED);
	
	while(!(ps2_hw_get_flags()&PS2_HW_FLAG_TRANSF_COMPLETE)){
	//wait
	}
	_delay_ms(5);
	ps2_hw_send_byte(0b00000101);
	
	
	uart_send_string("---");
	
	
	

    while(1){
	
		uint8_t data;
		int8_t ret;
		//uart_send_byte( ps2_hw_get_flags() );
		
		
		
		ret = ps2_hw_receive_byte(&data);
		if(ret != -1){
		
			uart_send_byte(ret);
			uart_send_byte(data);
			uart_send_byte( ps2_hw_get_flags() );
			uart_send_byte( 0xAA );
			uart_send_byte( 0xAA );
		}
		
		_delay_ms(1);
	
	
       //TODO translate
    }
}
