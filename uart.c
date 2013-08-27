#include <avr/io.h>

#include "uart.h"


#define UART_UBRR ((F_CPU/(16*UART_UART_BAUD))-1)


void uart_init( void )
{
	UBRR0 = (uint16_t)103; //FIXME: probably shouldn't be a constant...

    UCSR0B |= (1<<TXEN0);                   //enable Rx, disable interrupts
    UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);    //8n1
}


void uart_send_byte( uint8_t data )
{
    while(!(UCSR0A & (1<<UDRE0))); //copied from datasheet.

    UDR0 = data;
}


void uart_send_string( char *str )
{
    for(uint8_t i=0; str[i] != '\0'; i++){
        uart_send_byte(str[i]);
    }
}
