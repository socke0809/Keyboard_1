#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ps2.h"
#include "uart.h"


int main( void )
{
    uart_init();
	
	ps2_init();

	_delay_ms(1000); //dbg
	
	sei();
	
	


    uart_send_string("---");// WELCOME TO PS2 CONTROLLER ---\n");
	
	//uart_send_byte( ps2_hw_get_flags() );
	
	
	
	/*ps2_hw_send_byte(0xED);
	uart_send_byte( ps2_hw_get_flags() );
	//while(!(ps2_hw_get_flags()&PS2_HW_FLAG_TRANSF_COMPLETE));
   //_delay_ms(100);
	ps2_hw_send_byte(0x05);
	uart_send_byte( ps2_hw_get_flags() );
	//while(!(ps2_hw_get_flags()&PS2_HW_FLAG_TRANSF_COMPLETE));
	_delay_ms(100);
*/



	uart_send_string("---");
	
	
	
    while(1){
	
		//uint8_t data;
		//int8_t ret;
		char* keys; 
		
		keys = ps2_get_keys();
		
		uart_send_string(keys);
		uart_send_byte('\r');
		uart_send_byte( '\n');
		
		/*ret = ps2_hw_receive_byte(&data);
		
		if(ret != -1){
		
			uart_send_byte(ret);
			uart_send_byte(data);
			uart_send_byte( ps2_hw_get_flags() );
			
			uart_send_byte( 0xAA );
			uart_send_byte( 0xAA );
		}*/
		
		_delay_ms(1);
	
    }
}

