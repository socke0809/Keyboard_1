#ifndef __PS2_H__
#define __PS2_H__


#include <inttypes.h>



void ps2_init(void);

char* ps2_get_keys(void);

char ps2_get_new_key(void);



#endif