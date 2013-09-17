#include <avr/io.h>
#include <avr/interrupt.h>

#include "sound.h"

void sound_init(void){
	TCCR1A	 = 	0;
	TCCR1B 	|= 	(1<<CS10) | (1<<WGM12); //sets prescaler 1
	OCR1A	 = 	30000;	//timer stops counting at 30000
	TIMSK1 	|= 			(1<<OCIE1A); //enables interrupt
	SOUND_SIGNAL_DDR |= (1<<SOUND_SIGNAL); //sets sound_signal as output
	
}

ISR (TIMER1_COMPA_vect){
 SOUND_SIGNAL_PORT ^= (1<<SOUND_SIGNAL);
}

void set_OCR(char key){
	uint16_t f_oc = 0;
	switch(key){
		case 'c': f_oc = 264; break;
		case 'd': f_oc = 297; break;
		case 'e': f_oc = 330; break;
		case 'f': f_oc = 352; break;
		case 'g': f_oc = 396; break;
		case 'a': f_oc = 440; break;
		case 'h': f_oc = 495; break;
	}
	OCR1A = (16000000/(2*f_oc))-1;
}