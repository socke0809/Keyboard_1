#include <avr/io.h>
#include <avr/interrupt.h>



void sound_init(void){
	TCCR1A = 0
	TCCR1B |= (1<<CS10)  | (1<<WGM12); //sets prescaler 1
	OCR1A = 30000;	//timer stops counting at 30000
	TIMSK1 |= (1<<OCIE1A); //enables interrupt
}

