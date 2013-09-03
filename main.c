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
	
	
	
	
	ps2_send_byte(0xED);
	uart_send_byte( ps2_hw_get_flags() );
	while(!(ps2_hw_get_flags()&PS2_HW_FLAG_TRANSF_COMPLETE));
    _delay_ms(20);

	ps2_send_byte(0x05);
	uart_send_byte( ps2_hw_get_flags() );
	while(!(ps2_hw_get_flags()&PS2_HW_FLAG_TRANSF_COMPLETE));
	_delay_ms(20);




	uart_send_string("---");
	
	
	
    while(1){
	
		uint8_t data;
		int8_t ret;
		
		
		
		ret = ps2_receive_byte(&data);
		if(ret != -1){
		
			uart_send_byte(ret);
			uart_send_byte(data);
			uart_send_byte( ps2_hw_get_flags() );
			uart_send_byte( 0xAA );
			uart_send_byte( 0xAA );
		}
		
		_delay_ms(1);
	
    }
}
