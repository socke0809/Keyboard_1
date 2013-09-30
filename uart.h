#ifndef __UART_H__
#define __UART_H__

#include <inttypes.h>


#define UART_BAUD 9600


void uart_init( void );

void uart_send_byte( uint8_t data );

void uart_send_string( char *str );


#endif //__UART_H__

