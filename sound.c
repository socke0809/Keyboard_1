#include <avr/io.h>
#include <avr/interrupt.h>


#include "sound.h"
#include "ps2.h"


volatile uint16_t count_max;
volatile char string[25];


void sound_init(char strg[25]){
	TCCR1A	 = 	0;
	TCCR1B 	|= 	(1<<CS10) | (1<<WGM12); //sets prescaler 1
	OCR1A	 = 	30000;	//timer stops counting at 30000
	TIMSK1 	|= 			(1<<OCIE1A); //enables interrupt
	SOUND_SIGNAL_DDR |= (1<<SOUND_SIGNAL); //sets sound_signal as output
	for(uint8_t i = 0; i < 25; i++){
		string[i] = strg[i];
	}
}

ISR (TIMER1_COMPA_vect){
static uint16_t count = 0;
char key1, key2;
 SOUND_SIGNAL_PORT ^= (1<<SOUND_SIGNAL);
	if(count == count_max){
		count = 0;
		if(string[0] != '/0'){
			if(string[1] == ' '){
				key2 = string[0];
				key1 = 'x';
				for(uint8_t i = 0; i < 25; i++){
					if(i<23){
					string[i] = string[i+2];
					}else{
					string[i] = '/0';
					}
				}
			}else{
				key1 = string[0];
				key2 = string[1];
				for(uint8_t i = 0; i < 25; i++){
					if(i<22){
					string[i] = string[i+3];
					}else{
					string[i] = '/0';
					}
				}
			}
			set_OCR(key1, key2);
		}
	}
	count++;
	
}
		
		
 


void set_OCR(char key1, char key2)
{
	
	uint16_t f_oc = 0;
	switch(key1){
		case '#':
			switch(key2){
			case 'c': f_oc = 277; break;
			case 'd': f_oc = 311; break;
			case 'g': f_oc = 370; break;
			case 'a': f_oc = 415; break;
			case 'h': f_oc = 466; break;
			default: break;
			
			}
			break;
		case 'b':
			switch(key2){
			case 'd': f_oc = 277; break;
			case 'e': f_oc = 311; break;
			case 'f': f_oc = 370; break;
			case 'g': f_oc = 415; break;
			case 'a': f_oc = 466; break;
			default: break;
			}
			break;
		default:
			switch(key2){
			case 'c': f_oc = 264; break;
			case 'd': f_oc = 297; break;
			case 'e': f_oc = 330; break;
			case 'f': f_oc = 352; break;
			case 'g': f_oc = 396; break;
			case 'a': f_oc = 440; break;
			case 'h': f_oc = 495; break;
			default: break;
			}
			break;
	}
	OCR1A = (16000000/(2*f_oc))-1;
	count_max = (2*f_oc*SOUND_PERIOD)/1000;
}