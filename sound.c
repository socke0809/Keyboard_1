#include <avr/io.h>
#include <avr/interrupt.h>

#include "sound.h"

void sound_init(void){
	TCCR1A = 0;
	TCCR1B |= (1<<CS10) | (1<<WGM12); //sets prescaler 1
	OCR1A = 30000;	//timer stops counting at 30000
	TIMSK1 |= (1<<OCIE1A); //enables interrupt
	SOUND_SIGNAL_DDR |= (1<<SOUND_SIGNAL); //sets sound_signal as output
	
}

ISR (TIMER1_COMPA_vect)
{
	if(~(SOUND_SIGNAL_PIN & (1<<SOUND_SIGNAL))){
		SOUND_SIGNAL_PORT |= (1<<SOUND_SIGNAL);
	}
	else{
		SOUND_SIGNAL_PORT &=~(1<<SOUND_SIGNAL);
		}
	
}