#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ps2_hw.h"


int main()
{
	_delay_ms(5000); //dbg

    uart_init();

	sei();


    uart_send_string("--- WELCOME TO PS2 CONTROLLER ---\n");

    while(1){
       //TODO translate
    }
}
