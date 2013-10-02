#include <avr/io.h>
#include <avr/interrupt.h>


#include "sound.h"
#include "ps2.h"


volatile uint16_t count_max = 0;
volatile char string[25];
volatile uint8_t key_count = 0;



void sound_init(char *strg){
	TCCR1A	 = 	0;
	TCCR1B 	|= 	(1<<CS10) | (1<<WGM12); //sets prescaler 1
	OCR1A	 = 	30000;	//timer stops counting at 30000
	SOUND_SIGNAL_DDR |= (1<<SOUND_SIGNAL); //sets sound_signal as output
	for(uint8_t i = 0; i < 25; i++){
		string[i] = strg[i];
	}
	key_count = 0;
	count_max = 0;

	TIMSK1 	|= 	(1<<OCIE1A); //enables interrupt
}
	
	
ISR (TIMER1_COMPA_vect){
    static uint16_t count = 0;
    uint8_t end_of_note = 0;
    char key1, key2;
    char key3 = 1;
	char key4 = 0;
    SOUND_SIGNAL_PORT ^= (1<<SOUND_SIGNAL);
    if(count == count_max){
        count = 0;
        while(end_of_note == 0){
            switch(string[key_count]){
                case '\0': 
                    TIMSK1 &= 	~(1<<OCIE1A);
                    SOUND_SIGNAL_PORT |= (1<<SOUND_SIGNAL);
                    key_count = 0;
                    end_of_note = 1;
                    break;
                case '#': 
                case 'b': 
                    key1 = string[key_count];
                    key_count++;
                    break;
                case 'c':
                case 'd':
                case 'e':
                case 'f':
                case 'g':
                case 'a':
                case 'h':
                    key2 =string[key_count];
                    key_count++;
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
					if(key_count == 0 || string[key_count-1] != '\'' ){
                    key3 = string[key_count];
					}else{
					key4 = string[key_count];
					}
                    key_count++;
                    break; 
                case ' ': 
                    set_OCR(key1, key2, key3, key4);
                    key_count++;
                    end_of_note = 1;
                    break;
                default: key_count++; break;
            }
        }
    }else{
        count++;
    }
}



void set_OCR(char key1, char key2, char key3, char key4)
{
	uint8_t x = 1;
	uint8_t n = 0;
	uint16_t f_oc = 0;
	switch(key1){
		case '#':
			switch(key2){
			case 'c': f_oc = 277; break;
			case 'd': f_oc = 311; break;
			case 'f': f_oc = 370; break;
			case 'g': f_oc = 415; break;
			case 'a': f_oc = 466; break;
			default: f_oc = 1000; break;
			
			}
			break;
		case 'b':
			switch(key2){
			case 'd': f_oc = 277; break;
			case 'e': f_oc = 311; break;
			case 'g': f_oc = 370; break;
			case 'a': f_oc = 415; break;
			case 'h': f_oc = 466; break;
			//case ' ': break;
			default:  f_oc = 1000; break;
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
			default: f_oc = 1000; break;
			}
			break;
	}
	switch(key3){
			case '1': x = 1; break;
			case '2': x = 2; break;
			case '3': x = 3; break;
			case '4': x = 4; break;
			case '5': x = 5; break;
			case '6': x = 6; break;
			case '7': x = 7; break;
			case '8': x = 8; break;
			case '9': x = 9; break;
			default: x = 1; break;
			}
	switch (key4){
			case '0': n = 0; break;
			case '1': n = 1; break;
			case '2': n = 2; break;
			case '3': n = 3; break;
			case '4': n = 4; break;
			case '5': n = 5; break;
			case '6': n = 6; break;
			case '7': n = 7; break;
			case '8': n = 8; break;
			case '9': n = 9; break;
			default: n = 0; break;
			}
	
		
	OCR1A = (16000000/(2*f_oc))-1;
	count_max = (2*(uint32_t)f_oc*SOUND_PERIOD*(uint32_t)x*2^((uint32_t)n))/1000;
}

void sound_stop(void){
	TIMSK1 	&= 	~(1<<OCIE1A);
	
	for(uint8_t i = 0; i<25; i++){
		string[i] = 0;
	}

}

