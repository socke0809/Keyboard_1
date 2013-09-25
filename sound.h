#ifndef __SOUND_H__
#define __SOUND_H__

#include <inttypes.h>

#define SOUND_SIGNAL_DDR 	DDRB
#define SOUND_SIGNAL_PORT 	PORTB
#define SOUND_SIGNAL_PIN 	PINB 
#define SOUND_SIGNAL		PB0 //PIN 8 am arduino

#define SOUND_PERIOD		250 // in ms

void sound_init(char *strg);
void set_OCR(char key1, char key2, char key3);

#endif