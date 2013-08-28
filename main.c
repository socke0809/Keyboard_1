#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ps2_hw.h"


int main( void )
{
	_delay_ms(5000); //dbg

    uart_init();
	
	ps2_hw_init();

	sei();


    uart_send_string("--- WELCOME TO PS2 CONTROLLER ---\n");

    while(1){
	
		uint8_t data;
		
		uart_send_byte( ps2_hw_receive_byte(&data));
		uart_send_byte(data);
		uart_send_byte( ps2_hw_get_flags() );
		uart_send_byte( 0xAA );
		uart_send_byte( 0xAA );
				_delay_ms(5);
		
		
	
	
       //TODO translate
    }
}
