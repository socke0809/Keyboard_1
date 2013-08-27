#include <avr/io.h>
#include <avr/interrupt.h>

#include "ps2_hw.h"


int main()
{
    uart_init();

	sei();


    uart_send_string("--- WELCOME TO PS2 CONTROLLER ---");

    while(1){
       //TODO translate
		
    }
}
