#include "ps2.h"
#include "ps2_hw.h"

char ps2KeyArray[7];



void ps2_init(void){
	ps2_hw_init();
	for(uint8_t i=0; i<6; i++){
		ps2KeyArray[i] = 0;
	}
}

char* ps2_get_keys(void){

	uint8_t data;
	char actKey = 0;
	uint8_t breakCode = 0;
	
	
	if(ps2_buffer_peek(&data, &rcvBuffer) ==0){
		if(data == 0xf0){
			breakCode = 1;
			if( rcvBuffer.size >=2){
				ps2_buffer_read(&data, &rcvBuffer);
				breakCode = 1;
			}
			else{
			 return ps2KeyArray;
			}
		}
	}
	else{
		return ps2KeyArray;
	}
	if(ps2_buffer_read(&data, &rcvBuffer) == 0){
		switch(data){
			case 0x1c: actKey = 'a'; break; 
			case 0x32: actKey = 'b'; break;
			case 0x21: actKey = 'c'; break;
			case 0x23: actKey = 'd'; break;
			case 0x24: actKey = 'e'; break;
			case 0x2b: actKey = 'f'; break; 
			case 0x34: actKey = 'g'; break;
			case 0x33: actKey = 'h'; break;
			case 0x43: actKey = 'i'; break;
			case 0x3b: actKey = 'j'; break;
			case 0x42: actKey = 'k'; break; 
			case 0x4b: actKey = 'l'; break;
			case 0x3a: actKey = 'm'; break;
			case 0x31: actKey = 'n'; break;
			case 0x44: actKey = 'o'; break;
			case 0x4d: actKey = 'p'; break; 
			case 0x15: actKey = 'q'; break;
			case 0x2d: actKey = 'r'; break;
			case 0x1b: actKey = 's'; break;
			case 0x2c: actKey = 't'; break;
			case 0x3c: actKey = 'u'; break; 
			case 0x2a: actKey = 'v'; break;
			case 0x1d: actKey = 'w'; break;
			case 0x22: actKey = 'x'; break;
			case 0x35: actKey = 'y'; break;
			case 0x1a: actKey = 'z'; break; 
			case 0x45: actKey = '0'; break;
			case 0x16: actKey = '1'; break;
			case 0x1e: actKey = '2'; break;
			case 0x26: actKey = '3'; break;
			case 0x25: actKey = '4'; break; 
			case 0x2e: actKey = '5'; break;
			case 0x36: actKey = '6'; break;
			case 0x3d: actKey = '7'; break;
			case 0x3e: actKey = '8'; break;
			case 0x46: actKey = '9'; break;
			case 0x5d: actKey = '#'; break;
			default: break;
		}
		
		for(uint8_t i = 0; i<6; i++){
			if(ps2KeyArray[i] == actKey){
				if(breakCode == 1){
					
					for(uint8_t j = i; j<6; j++){
						ps2KeyArray[j] = ps2KeyArray[j+1];
						}
					}

				break;
			}

			if(ps2KeyArray[i] == 0){
				ps2KeyArray[i] = actKey;
				ps2KeyArray[i+1] = 0;
				break;
			}
		}
		
		

	}

	return ps2KeyArray;
}



char get_new_key(void){

	uint8_t data;
	char actKey = 0;
	uint8_t breakCode = 0;


	if(ps2_buffer_peek(&data, &rcvBuffer) ==0){
		if(data == 0xf0){
			breakCode = 1;
			if( rcvBuffer.size >=2){
				ps2_buffer_read(&data, &rcvBuffer);
				breakCode = 1;
			}else{
				return 0;
			}
		}
	}else{
		return 0;
	}
	if(ps2_buffer_read(&data, &rcvBuffer) == 0){
		switch(data){
			case 0x1c: actKey = 'a'; break; 
			case 0x32: actKey = 'b'; break;
			case 0x21: actKey = 'c'; break;
			case 0x23: actKey = 'd'; break;
			case 0x24: actKey = 'e'; break;
			case 0x2b: actKey = 'f'; break; 
			case 0x34: actKey = 'g'; break;
			case 0x33: actKey = 'h'; break;
			case 0x43: actKey = 'i'; break;
			case 0x3b: actKey = 'j'; break;
			case 0x42: actKey = 'k'; break; 
			case 0x4b: actKey = 'l'; break;
			case 0x3a: actKey = 'm'; break;
			case 0x31: actKey = 'n'; break;
			case 0x44: actKey = 'o'; break;
			case 0x4d: actKey = 'p'; break; 
			case 0x15: actKey = 'q'; break;
			case 0x2d: actKey = 'r'; break;
			case 0x1b: actKey = 's'; break;
			case 0x2c: actKey = 't'; break;
			case 0x3c: actKey = 'u'; break; 
			case 0x2a: actKey = 'v'; break;
			case 0x1d: actKey = 'w'; break;
			case 0x22: actKey = 'x'; break;
			case 0x35: actKey = 'y'; break;
			case 0x1a: actKey = 'z'; break; 
			case 0x45: actKey = '0'; break;
			case 0x16: actKey = '1'; break;
			case 0x1e: actKey = '2'; break;
			case 0x26: actKey = '3'; break;
			case 0x25: actKey = '4'; break; 
			case 0x2e: actKey = '5'; break;
			case 0x36: actKey = '6'; break;
			case 0x3d: actKey = '7'; break;
			case 0x3e: actKey = '8'; break;
			case 0x46: actKey = '9'; break;
			case 0x5d: actKey = '#'; break;
			
			default: actKey = data;
			break;
		}
		
		if(breakCode == 0){
			return actKey;
		}
	}
	
	return 0;
}
	






